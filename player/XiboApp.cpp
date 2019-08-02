#include "XiboApp.hpp"

#include "MainLoop.hpp"
#include "config.hpp"

#include "utils/ScreenShoter.hpp"
#include "utils/Resources.hpp"
#include "xmlsink/XmlLoggerSink.hpp"

#include "control/MainWindowController.hpp"
#include "control/MainWindow.hpp"
#include "control/StatusScreenFormatter.hpp"
#include "control/StatusScreen.hpp"
#include "control/media/player/video/XiboVideoSink.hpp"
#include "control/media/creators/MediaParsersRepo.hpp"
#include "control/media/creators/MediaFactoriesRepo.hpp"
#include "control/layout/LayoutsManager.hpp"

#include "managers/CollectionInterval.hpp"
#include "managers/FileCacheManager.hpp"
#include "managers/XmrManager.hpp"

#include "schedule/ScheduleSerializer.hpp"
#include "schedule/XiboLayoutScheduler.hpp"

#include "networking/WebServer.hpp"
#include "networking/HttpClient.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"
#include "networking/xmds/SoapRequestSender.hpp"

#include "common/settings/PlayerSettings.hpp"
#include "common/settings/CmsSettings.hpp"
#include "common/crypto/RsaManager.hpp"

#include <gst/gst.h>
#include <glibmm/main.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <boost/date_time/time_clock.hpp>

std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    auto logger = XiboLogger::create(SpdLogger, createLoggerSinks());
    logger->setLevel(LoggingLevel::Debug);
    logger->setPattern("[%H:%M:%S.%e] [%t] [%l]: %v");

    gst_init(nullptr, nullptr);
    registerVideoSink();
    Resources::setDirectory(ProjectResources::defaultResourcesDir());

    m_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *m_app;
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
    if(!gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "Xibo Video Sink", "Video Sink Plugin for gstreamer",
                                   pluginInit, "0.5", "GPL", "source", "package", "http://github.com/Stivius"))
    {
        throw std::runtime_error("XiboVideoSink was not registered");
    }
}

XiboApp::XiboApp(const std::string& name) :
    m_mainLoop(std::make_unique<MainLoop>(name)),
    m_scheduler(std::make_unique<XiboLayoutScheduler>()),
    m_scheduleSerializer(std::make_unique<ScheduleSerializer>()),
    m_fileManager(std::make_unique<FileCacheManager>()),
    m_xmrManager(std::make_unique<XmrManager>()),
    m_webserver(std::make_shared<XiboWebServer>()),
    m_layoutsManager(std::make_unique<LayoutsManager>(*m_scheduler))
{
    if(!FileSystem::exists(ProjectResources::cmsSettingsFile()))
        throw std::runtime_error("Update CMS settings using player options app");

    m_cmsSettings.loadFrom(ProjectResources::cmsSettingsFile());
    m_playerSettings.loadFrom(ProjectResources::playerSettingsFile());
    Resources::setDirectory(FilePath{m_cmsSettings.resourcesPath});

    FilePath scheduleFile{ProjectResources::configDirectory() / DefaultScheduleFile};
    m_scheduler->reloadSchedule(m_scheduleSerializer->parseSchedule(scheduleFile));
    m_fileManager->loadCache(Resources::directory() / DefaultCacheFile);
    m_webserver->setRootDirectory(Resources::directory());
    m_webserver->run(m_playerSettings.embeddedServerPort);
    Log::debug(m_webserver->address());
    HttpClient::instance().setProxyServer(m_cmsSettings.domain, m_cmsSettings.username, m_cmsSettings.password);
    RsaManager::instance().load();
    setupXmrManager();

    MediaParsersRepo::init();
    MediaFactoriesRepo::init();

    m_mainLoop->setShutdownAction([this](){
        m_layoutsManager.reset();
        m_xmrManager->stop();
        HttpClient::instance().shutdown();
        if(m_collectionInterval)
        {
            m_collectionInterval->stop();
        }
    });
}

void XiboApp::setupXmrManager()
{
    m_xmrManager->collectionInterval().connect([this](){
        Log::info("Start unscheduled collection");

        m_collectionInterval->collect([this](const PlayerError& error){
            onCollectionFinished(error);
        });
    });

    m_xmrManager->screenshot().connect([this](){
        Log::info("Taking unscheduled screenshot");

        Managers::screenShoter().takeBase64([this](const std::string& screenshot){
            m_xmdsManager->submitScreenShot(screenshot).then([](auto future){
                auto [error, result] = future.get();
                if(error)
                {
                    Log::error("SubmitScreenShot: {}", error);
                }
            });
        });
    });
}

XiboApp::~XiboApp()
{
    if(gst_is_initialized())
    {
        gst_deinit();
    }
}

XiboApp& XiboApp::app()
{
    return *m_app;
}

FileCacheManager& XiboApp::fileManager()
{
    return *m_fileManager;
}

ScreenShoter& XiboApp::screenShoter()
{
    return *m_screenShoter;
}

XiboWebServer& XiboApp::webserver()
{
    return *m_webserver;
}

int XiboApp::run()
{
    m_mainWindow = std::make_shared<MainWindow>();
    m_windowController = std::make_unique<MainWindowController>(m_mainWindow, *m_layoutsManager);

    auto statusScreen = std::make_shared<StatusScreen>(640, 480);
    m_windowController->statusScreenRequested().connect([this, statusScreen](){
        StatusInfo info{collectGeneralInfo(), m_collectionInterval->status(), m_scheduler->status(), m_xmrManager->status()};

        statusScreen->setText(StatusScreenFormatter::formatInfo(info));
        statusScreen->show();
    });

    m_screenShoter = std::make_unique<ScreenShoter>(*m_mainWindow);
    m_xmdsManager = std::make_unique<XmdsRequestSender>(m_cmsSettings.cmsAddress, m_cmsSettings.key, m_cmsSettings.displayId);
    m_collectionInterval = createCollectionInterval(*m_xmdsManager);

    applyPlayerSettings(m_playerSettings);

    m_collectionInterval->startRegularCollection();
    m_layoutsManager->fetchAllLayouts();

    m_mainWindow->showAll();

    return m_mainLoop->run(*m_mainWindow);
}

GeneralInfo XiboApp::collectGeneralInfo()
{
    GeneralInfo info;

    info.currentDT = DateTimeProvider::now();
    info.cmsAddress = m_cmsSettings.cmsAddress;
    info.resourcesPath = m_cmsSettings.resourcesPath;
    info.codeVersion = ProjectResources::codeVersion();
    info.projectVersion = ProjectResources::version();
    info.screenShotInterval = m_playerSettings.collectInterval;
    info.displayName = m_playerSettings.displayName;
    info.windowWidth = m_mainWindow->width();
    info.windowHeight = m_mainWindow->height();

    return info;
}


std::unique_ptr<CollectionInterval> XiboApp::createCollectionInterval(XmdsRequestSender& xmdsManager)
{
    auto interval = std::make_unique<CollectionInterval>(xmdsManager);

    interval->collectionFinished().connect(sigc::mem_fun(this, &XiboApp::onCollectionFinished));
    interval->settingsUpdated().connect(sigc::mem_fun(this, &XiboApp::updateAndApplySettings));
    interval->scheduleUpdated().connect([this](const Schedule::Result& result){
        m_scheduler->reloadSchedule(m_scheduleSerializer->parseSchedule(result.scheduleXml));
    });

    return interval;
}

void XiboApp::onCollectionFinished(const PlayerError& error)
{
    if(error)
    {
        Log::error("[Collection interval] {}", error);
    }
    else
    {
        if(m_scheduler->currentLayoutId() == EmptyLayoutId)
        {
            m_layoutsManager->fetchMainLayout(m_scheduler->nextLayoutId());
        }
    }
}

void XiboApp::updateAndApplySettings(const PlayerSettings& settings)
{
    applyPlayerSettings(settings);

    m_playerSettings = std::move(settings);
    m_playerSettings.saveTo(ProjectResources::playerSettingsFile());
}

void XiboApp::applyPlayerSettings(const PlayerSettings& settings)
{
    Log::logger()->setLevel(settings.logLevel);
    m_collectionInterval->updateInterval(settings.collectInterval);
    m_xmrManager->connect(settings.xmrNetworkAddress);
    PlayerSettings::Dimensions dimensions{settings.width, settings.height, settings.x, settings.y};
    m_windowController->updateWindowDimensions(dimensions);

    Log::debug("Player settings updated");
}
