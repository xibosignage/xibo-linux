#include "XiboApp.hpp"

#include "MainLoop.hpp"
#include "config.hpp"

#include "common/fs/Resources.hpp"
#include "utils/ScreenShoter.hpp"
#include "xmlsink/XmlLoggerSink.hpp"

#include "control/MainWindow.hpp"
#include "control/MainWindowController.hpp"
#include "control/StatusScreen.hpp"
#include "control/StatusScreenFormatter.hpp"
#include "control/layout/LayoutsManager.hpp"
#include "control/media/MediaCreatorsRepo.hpp"
#include "control/media/player/video/XiboVideoSink.hpp"

#include "managers/CollectionInterval.hpp"
#include "managers/XmrManager.hpp"

#include "schedule/ScheduleParser.hpp"
#include "schedule/ScheduleSerializer.hpp"
#include "schedule/Scheduler.hpp"

#include "networking/HttpClient.hpp"
#include "networking/WebServer.hpp"
#include "networking/xmds/SoapRequestSender.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"

#include "common/System.hpp"
#include "common/crypto/RsaManager.hpp"
#include "common/fs/FileCache.hpp"
#include "common/settings/CmsSettings.hpp"
#include "common/settings/PlayerSettings.hpp"

#include <boost/date_time/time_clock.hpp>
#include <glibmm/main.h>
#include <gst/gst.h>
#include <spdlog/sinks/stdout_sinks.h>

static std::unique_ptr<XiboApp> g_app;

XiboApp& xiboApp()
{
    return *g_app;
}

XiboApp& XiboApp::create(const std::string& name)
{
    auto logger = XiboLogger::create(SpdLogger, createLoggerSinks());
    logger->setLevel(LoggingLevel::Debug);
    logger->setPattern("[%H:%M:%S.%e] [%t] [%l]: %v");

    gst_init(nullptr, nullptr);
    registerVideoSink();
    Resources::setDirectory(ProjectResources::defaultResourcesDir());

    g_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *g_app;
}

std::vector<spdlog::sink_ptr> XiboApp::createLoggerSinks()
{
    std::vector<spdlog::sink_ptr> sinks;

    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    sinks.push_back(std::make_shared<LoggerXmlSinkMt>(XmlLogsRepo::get()));

    return sinks;
}

void XiboApp::registerVideoSink()
{
    if (!gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "Xibo Video Sink",
                                    "Video Sink Plugin for gstreamer", pluginInit, "0.5", "GPL", "source", "package",
                                    "http://github.com/Stivius"))
    {
        throw std::runtime_error("XiboVideoSink was not registered");
    }
}

XiboApp::XiboApp(const std::string& name) :
    mainLoop_(std::make_unique<MainLoop>(name)),
    fileCache_(std::make_unique<FileCache>()),
    scheduler_(std::make_unique<Scheduler>(*fileCache_)),
    xmrManager_(std::make_unique<XmrManager>()),
    webserver_(std::make_shared<XiboWebServer>()),
    layoutsManager_(std::make_unique<LayoutsManager>(*scheduler_))
{
    if (!FileSystem::exists(ProjectResources::cmsSettingsPath()))
        throw std::runtime_error("Update CMS settings using player options app");

    System sys;
    sys.preventSleep();

    cmsSettings_.loadFrom(ProjectResources::cmsSettingsPath());
    playerSettings_.loadFrom(ProjectResources::playerSettingsPath());
    Resources::setDirectory(FilePath{cmsSettings_.resourcesPath});

    fileCache_->loadFrom(ProjectResources::cachePath());

    webserver_->setRootDirectory(Resources::directory());
    webserver_->run(playerSettings_.embeddedServerPort);

    Log::debug(webserver_->address());
    HttpClient::instance().setProxyServer(cmsSettings_.domain, cmsSettings_.username, cmsSettings_.password);
    RsaManager::instance().load();
    setupXmrManager();

    MediaCreatorsRepo::init();

    mainLoop_->setShutdownAction([this]() {
        layoutsManager_.reset();
        HttpClient::instance().shutdown();
        if (collectionInterval_)
        {
            collectionInterval_->stop();
        }
    });
}

void XiboApp::setupXmrManager()
{
    xmrManager_->collectionInterval().connect([this]() {
        Log::info("[CollectionInterval] Start unscheduled collection");

        collectionInterval_->collect([this](const PlayerError& error) { onCollectionFinished(error); });
    });

    xmrManager_->screenshot().connect([this]() {
        Log::info("Taking unscheduled screenshot");

        Managers::screenShoter().takeBase64([this](const std::string& screenshot) {
            xmdsManager_->submitScreenShot(screenshot).then([](auto future) {
                auto [error, result] = future.get();
                boost::ignore_unused(result);
                if (error)
                {
                    Log::error("[SubmitScreenShot] {}", error);
                }
            });
        });
    });
}

XiboApp::~XiboApp()
{
    if (gst_is_initialized())
    {
        gst_deinit();
    }
}

FileCache& XiboApp::fileManager()
{
    return *fileCache_;
}

ScreenShoter& XiboApp::screenShoter()
{
    return *screenShoter_;
}

XiboWebServer& XiboApp::webserver()
{
    return *webserver_;
}

int XiboApp::run()
{
    mainWindow_ = std::make_shared<MainWindow>();
    windowController_ = std::make_unique<MainWindowController>(mainWindow_, *layoutsManager_);

    auto statusScreen = std::make_shared<StatusScreen>(640, 480);
    windowController_->statusScreenRequested().connect([this, statusScreen]() {
        StatusInfo info{collectGeneralInfo(), collectionInterval_->status(), scheduler_->status(),
                        xmrManager_->status()};

        statusScreen->setText(StatusScreenFormatter::formatInfo(info));
        statusScreen->show();
    });

    screenShoter_ = std::make_unique<ScreenShoter>(*mainWindow_);
    xmdsManager_ =
        std::make_unique<XmdsRequestSender>(cmsSettings_.cmsAddress, cmsSettings_.key, cmsSettings_.displayId);
    collectionInterval_ = createCollectionInterval(*xmdsManager_);

    applyPlayerSettings(playerSettings_);

    ScheduleParser parser;
    scheduler_->reloadSchedule(parser.scheduleFrom(ProjectResources::schedulePath()));
    scheduler_->scheduleUpdated().connect([](const LayoutSchedule& schedule) {
        ScheduleSerializer serializer;
        serializer.scheduleTo(schedule, ProjectResources::schedulePath());
    });

    collectionInterval_->startRegularCollection();
    mainWindow_->showAll();

    return mainLoop_->run(*mainWindow_);
}

GeneralInfo XiboApp::collectGeneralInfo()
{
    GeneralInfo info;

    info.currentDateTime = DateTime::now();
    info.cmsAddress = cmsSettings_.cmsAddress;
    info.resourcesPath = cmsSettings_.resourcesPath;
    info.codeVersion = ProjectResources::codeVersion();
    info.projectVersion = ProjectResources::version();
    info.screenShotInterval = playerSettings_.collectInterval;
    info.displayName = playerSettings_.displayName;
    info.windowWidth = mainWindow_->width();
    info.windowHeight = mainWindow_->height();

    return info;
}

std::unique_ptr<CollectionInterval> XiboApp::createCollectionInterval(XmdsRequestSender& xmdsManager)
{
    auto interval = std::make_unique<CollectionInterval>(xmdsManager);

    interval->collectionFinished().connect(std::bind(&XiboApp::onCollectionFinished, this, ph::_1));
    interval->settingsUpdated().connect(std::bind(&XiboApp::updateAndApplySettings, this, ph::_1));
    interval->scheduleAvailable().connect([this](const Schedule::Result& result) {
        ScheduleParser parser;
        scheduler_->reloadSchedule(parser.scheduleFrom(result.scheduleXml));
    });
    interval->filesDownloaded().connect([this]() { scheduler_->reloadQueue(); });

    return interval;
}

void XiboApp::onCollectionFinished(const PlayerError& error)
{
    if (error)
    {
        Log::error("[CollectionInterval] {}", error);
    }
}

void XiboApp::updateAndApplySettings(const PlayerSettings& settings)
{
    applyPlayerSettings(settings);

    playerSettings_ = std::move(settings);
    playerSettings_.saveTo(ProjectResources::playerSettingsPath());
}

void XiboApp::applyPlayerSettings(const PlayerSettings& settings)
{
    Log::logger()->setLevel(settings.logLevel);
    collectionInterval_->updateInterval(settings.collectInterval);
    xmrManager_->connect(settings.xmrNetworkAddress);
    PlayerSettings::Dimensions dimensions{settings.width, settings.height, settings.x, settings.y};
    windowController_->updateWindowDimensions(dimensions);

    Log::debug("Player settings updated");
}
