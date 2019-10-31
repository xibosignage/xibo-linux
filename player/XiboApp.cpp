#include "XiboApp.hpp"

#include "MainLoop.hpp"
#include "config.hpp"
#include "constants.hpp"

#include "control/ApplicationWindow.hpp"
#include "control/layout/LayoutsManager.hpp"
#include "control/media/MediaParsersRepo.hpp"

#include "managers/CollectionInterval.hpp"
#include "managers/StatsRecorder.hpp"
#include "managers/XmrManager.hpp"
#include "schedule/Scheduler.hpp"
#include "screenshot/ScreeShoterFactory.hpp"
#include "screenshot/ScreenShotInterval.hpp"

#include "networking/HttpClient.hpp"
#include "networking/WebServer.hpp"
#include "networking/xmds/SoapRequestSender.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"

#include "common/PlayerRuntimeError.hpp"
#include "common/crypto/RsaManager.hpp"
#include "common/fs/FileCacheImpl.hpp"
#include "common/fs/Resources.hpp"
#include "common/logger/Logging.hpp"
#include "common/settings/CmsSettings.hpp"
#include "common/settings/PlayerSettings.hpp"
#include "common/system/System.hpp"

#include <gst/gst.h>

static std::unique_ptr<XiboApp> g_app;

XiboApp& xiboApp()
{
    return *g_app;
}

XiboApp& XiboApp::create(const std::string& name)
{
    Log::create();
    gst_init(nullptr, nullptr);
    Resources::setDirectory(ProjectResources::defaultResourcesDir());

    g_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *g_app;
}

XiboApp::XiboApp(const std::string& name) :
    mainLoop_(std::make_unique<MainLoop>(name)),
    fileCache_(std::make_unique<FileCacheImpl>()),
    scheduler_(std::make_unique<Scheduler>(*fileCache_)),
    statsRecorder_(std::make_unique<StatsRecorder>()),
    xmrManager_(std::make_unique<XmrManager>()),
    webserver_(std::make_shared<XiboWebServer>()),
    layoutsManager_(std::make_unique<LayoutsManager>(*scheduler_, *statsRecorder_, *fileCache_))
{
    if (!FileSystem::exists(ProjectResources::cmsSettingsPath()))
        throw PlayerRuntimeError{"XiboApp", "Update CMS settings using player options app"};

    System::preventSleep();

    cmsSettings_.loadFrom(ProjectResources::cmsSettingsPath());
    playerSettings_.fromFile(ProjectResources::playerSettingsPath());
    Resources::setDirectory(FilePath{cmsSettings_.resourcesPath});

    fileCache_->loadFrom(ProjectResources::cachePath());

    webserver_->setRootDirectory(Resources::directory());
    webserver_->run(playerSettings_.embeddedServerPort);

    Log::info("[XiboApp] {}", webserver_->address());
    HttpClient::instance().setProxyServer(cmsSettings_.proxy());
    RsaManager::instance().load();
    setupXmrManager();

    MediaParsersRepo::init();

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
        CHECK_UI_THREAD();

        Log::info("[XMR] Start unscheduled collection");

        collectionInterval_->collect([this](const PlayerError& error) { onCollectionFinished(error); });
    });

    xmrManager_->screenshot().connect([this]() {
        CHECK_UI_THREAD();

        Log::info("[XMR] Taking unscheduled screenshot");

        screenShoter_->takeBase64([this](const std::string& screenshot) {
            xmdsManager_->submitScreenShot(screenshot).then([](auto future) {
                auto [error, result] = future.get();
                if (error)
                {
                    Log::error("[XMDS::SubmitScreenShot] {}", error);
                }
                else
                {
                    std::string message = result.success ? "Submitted" : "Not submitted";
                    Log::debug("[XMDS::SubmitScreenShot] {}", message);
                }
            });
        });
    });
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
    mainWindow_ = std::make_unique<ApplicationWindowGtk>();
    setupLayoutManager();

    mainWindow_->statusScreenShown().connect([this]() {
        CHECK_UI_THREAD();

        StatusInfo info{
            collectGeneralInfo(), collectionInterval_->status(), scheduler_->status(), xmrManager_->status()};

        mainWindow_->updateStatusScreen(info);
    });

    screenShoter_ = ScreenShoterFactory::create(*mainWindow_);
    xmdsManager_ =
        std::make_unique<XmdsRequestSender>(cmsSettings_.cmsAddress, cmsSettings_.key, cmsSettings_.displayId);
    screenShotInterval_ = std::make_unique<ScreenShotInterval>(*xmdsManager_, *screenShoter_);
    collectionInterval_ = createCollectionInterval(*xmdsManager_);

    applyPlayerSettings(playerSettings_);

    scheduler_->reloadSchedule(LayoutSchedule::fromFile(ProjectResources::schedulePath()));
    scheduler_->scheduleUpdated().connect(
        [](const LayoutSchedule& schedule) { schedule.toFile(ProjectResources::schedulePath()); });

    collectionInterval_->startRegularCollection();
    mainWindow_->showAll();

    return mainLoop_->run(*mainWindow_);
}

void XiboApp::setupLayoutManager()
{
    layoutsManager_->mainLayoutFetched().connect([this](const MainLayoutWidget& layout) {
        if (layout)
        {
            mainWindow_->setMainLayout(layout);
            layout->showAll();
        }
        else
        {
            mainWindow_->showSplashScreen();
        }
    });

    layoutsManager_->overlaysFetched().connect([this](const OverlaysWidgets& overlays) {
        mainWindow_->setOverlays(overlays);
        for (auto&& layout : overlays)
        {
            layout->showAll();
        }
    });
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
    auto interval = std::make_unique<CollectionInterval>(xmdsManager, *statsRecorder_);

    interval->collectionFinished().connect(std::bind(&XiboApp::onCollectionFinished, this, ph::_1));
    interval->settingsUpdated().connect(std::bind(&XiboApp::updateAndApplySettings, this, ph::_1));
    interval->scheduleAvailable().connect([this](const Schedule::Result& result) {
        CHECK_UI_THREAD();

        scheduler_->reloadSchedule(LayoutSchedule::fromString(result.scheduleXml));
    });
    interval->filesDownloaded().connect([this]() {
        CHECK_UI_THREAD();

        scheduler_->reloadQueue();
    });

    return interval;
}

void XiboApp::onCollectionFinished(const PlayerError& error)
{
    CHECK_UI_THREAD();

    if (error)
    {
        Log::error("[CollectionInterval] {}", error);
    }
}

void XiboApp::updateAndApplySettings(const PlayerSettings& settings)
{
    CHECK_UI_THREAD();

    applyPlayerSettings(settings);

    playerSettings_ = std::move(settings);
    playerSettings_.saveTo(ProjectResources::playerSettingsPath());
}

void XiboApp::applyPlayerSettings(const PlayerSettings& settings)
{
    try
    {
        Log::setLevel(settings.logLevel);
        collectionInterval_->updateInterval(settings.collectInterval);
        xmrManager_->connect(settings.xmrNetworkAddress);
        mainWindow_->setSize(settings.width, settings.height);
        mainWindow_->move(settings.x, settings.y);
        screenShotInterval_->updateInterval(settings.screenshotInterval);

        Log::debug("[XiboApp] Player settings updated");
    }
    catch (std::exception& e)
    {
        Log::error("[XiboApp] Error while upadting settings: {}", e.what());
    }
}
