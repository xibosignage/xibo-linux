#include "XiboApp.hpp"
#include "MainLoop.hpp"

#include "config/AppConfig.hpp"
#include "config/CmsSettings.hpp"
#include "config/PlayerSettings.hpp"

#include "control/ApplicationWindow.hpp"
#include "control/layout/LayoutsManager.hpp"
#include "control/media/MediaParsersRepo.hpp"
#include "control/media/webview/LocalWebServer.hpp"
#include "control/screenshot/ScreeShoterFactory.hpp"
#include "control/screenshot/ScreenShotInterval.hpp"

#include "schedule/Scheduler.hpp"
#include "stat/Recorder.hpp"
#include "xmr/XmrManager.hpp"

#include "cms/CollectionInterval.hpp"
#include "cms/xmds/SoapRequestSender.hpp"
#include "cms/xmds/XmdsRequestSender.hpp"
#include "networking/HttpClient.hpp"

#include "common/PlayerRuntimeError.hpp"
#include "common/crypto/RsaManager.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/logger/Logging.hpp"
#include "common/storage/FileCacheImpl.hpp"
#include "common/system/System.hpp"

static std::unique_ptr<XiboApp> g_app;

XiboApp& xiboApp()
{
    return *g_app;
}

XiboApp& XiboApp::create(const std::string& name)
{
    Log::create();

    g_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *g_app;
}

XiboApp::XiboApp(const std::string& name) :
    mainLoop_(std::make_unique<MainLoop>(name)),
    fileCache_(std::make_unique<FileCacheImpl>()),
    scheduler_(std::make_unique<Scheduler>(*fileCache_)),
    statsRecorder_(std::make_unique<Stats::Recorder>()),
    webserver_(std::make_shared<LocalWebServer>())
{
    if (!FileSystem::exists(AppConfig::cmsSettingsPath()))
        throw PlayerRuntimeError{"XiboApp", "Update CMS settings using player options app"};

    playerSettings_.logLevel().valueChanged().connect([](const std::string& logLevel) { Log::setLevel(logLevel); });

    cmsSettings_.fromFile(AppConfig::cmsSettingsPath());
    playerSettings_.fromFile(AppConfig::playerSettingsPath());
    fileCache_->loadFrom(AppConfig::cachePath());

    System::preventSleep();
    AppConfig::resourceDirectory(cmsSettings_.resourcesPath());
    checkResourceDirectory();

    webserver_->setRootDirectory(cmsSettings_.resourcesPath());
    webserver_->run(playerSettings_.embeddedServerPort());

    HttpClient::instance().setProxyServer(cmsSettings_.proxy());
    RsaManager::instance().load();
    xmrManager_ = createXmrManager();

    MediaParsersRepo::init();

    mainLoop_->setShutdownAction([this]() {
        layoutManager_.reset();
        xmrManager_->stop();
        HttpClient::instance().shutdown();
        if (collectionInterval_)
        {
            collectionInterval_->stop();
        }
    });
}

std::unique_ptr<XmrManager> XiboApp::createXmrManager()
{
    auto xmrChannel = XmrChannel::fromCmsSettings(cmsSettings_.address(), cmsSettings_.key(), cmsSettings_.displayId());
    auto manager = std::make_unique<XmrManager>(xmrChannel);

    manager->connect(playerSettings_.xmrNetworkAddress());
    playerSettings_.xmrNetworkAddress().valueChanged().connect(std::bind(&XmrManager::connect, manager.get(), ph::_1));

    manager->collectionInterval().connect([this]() {
        CHECK_UI_THREAD();
        Log::info("[XMR] Start unscheduled collection");

        collectionInterval_->collectNow();
    });

    manager->screenshot().connect([this]() {
        CHECK_UI_THREAD();
        Log::info("[XMR] Taking unscheduled screenshot");

        screenShotInterval_->takeScreenShot();
    });

    return manager;
}

int XiboApp::run()
{
    mainWindow_ = createMainWindow();
    layoutManager_ = createLayoutManager();

    playerSettings_.statsEnabled().valueChanged().connect(
        [this](bool statsEnabled) { layoutManager_->statsEnabled(statsEnabled); });

    xmdsManager_ =
        std::make_unique<XmdsRequestSender>(cmsSettings_.address(), cmsSettings_.key(), cmsSettings_.displayId());
    screenShotInterval_ = createScreenshotInterval(*xmdsManager_, *mainWindow_);
    collectionInterval_ = createCollectionInterval(*xmdsManager_);

    collectionInterval_->setCurrentLayoutId(scheduler_->currentLayoutId());

    scheduler_->layoutUpdated().connect(
        [this]() { collectionInterval_->setCurrentLayoutId(scheduler_->currentLayoutId()); });

    scheduler_->reloadSchedule(LayoutSchedule::fromFile(AppConfig::schedulePath()));
    scheduler_->scheduleUpdated().connect(
        [](const LayoutSchedule& schedule) { schedule.toFile(AppConfig::schedulePath()); });

    mainLoop_->started().connect([this] {
        collectionInterval_->collectNow();
        mainWindow_->showAll();
    });

    return mainLoop_->run(*mainWindow_);
}

// FIXME temporary workaround until plugin init
Uri XiboApp::localAddress()
{
    return g_app->webserver_->address();
}

std::shared_ptr<ApplicationWindowGtk> XiboApp::createMainWindow()
{
    std::shared_ptr<ApplicationWindowGtk> window = ApplicationWindowGtk::create(playerSettings_.size().width(),
                                                                                playerSettings_.size().height(),
                                                                                playerSettings_.position().x(),
                                                                                playerSettings_.position().y());

    playerSettings_.size().valueChanged().connect([window](int width, int height) { window->setSize(width, height); });
    playerSettings_.position().valueChanged().connect([window](int x, int y) { window->move(x, y); });

    window->statusScreenShown().connect([this, window]() {
        CHECK_UI_THREAD();
        StatusInfo info{
            collectGeneralInfo(), collectionInterval_->status(), scheduler_->status(), xmrManager_->status()};

        window->updateStatusScreen(info, fileCache_->invalidFiles());
    });

    window->exitWithoutRestartRequested().connect([]() { System::terminateProccess(System::parentProcessId()); });

    return window;
}

std::unique_ptr<LayoutsManager> XiboApp::createLayoutManager()
{
    auto manager =
        std::make_unique<LayoutsManager>(*scheduler_, *statsRecorder_, *fileCache_, playerSettings_.statsEnabled());

    manager->mainLayoutFetched().connect([this](const MainLayoutWidget& layout) {
        CHECK_UI_THREAD();
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

    manager->overlaysFetched().connect([this](const OverlaysWidgets& overlays) {
        CHECK_UI_THREAD();
        mainWindow_->setOverlays(overlays);
        for (auto&& layout : overlays)
        {
            layout->showAll();
        }
    });

    return manager;
}

GeneralInfo XiboApp::collectGeneralInfo()
{
    GeneralInfo info;

    info.currentDateTime = DateTime::now();
    info.cmsAddress = cmsSettings_.address();
    info.resourcesPath = cmsSettings_.resourcesPath();
    info.codeVersion = AppConfig::codeVersion();
    info.projectVersion = AppConfig::releaseVersion();
    info.screenShotInterval = playerSettings_.screenshotInterval();
    info.displayName = playerSettings_.displayName();
    info.windowWidth = mainWindow_->width();
    info.windowHeight = mainWindow_->height();

    return info;
}

void XiboApp::checkResourceDirectory()
{
    for (auto&& file : fileCache_->cachedFiles())
    {
        if (!fileCache_->valid(file)) continue;

        auto fullPath = cmsSettings_.resourcesPath() / file;
        if (!FileSystem::exists(fullPath) || !fileCache_->cached(file, Md5Hash::fromFile(fullPath)))
        {
            Log::trace("[{}] Missing/corrupted in resource directory", file);
            fileCache_->markAsInvalid(file);
        }
    }
}

std::unique_ptr<CollectionInterval> XiboApp::createCollectionInterval(XmdsRequestSender& xmdsManager)
{
    auto interval = std::make_unique<CollectionInterval>(
        xmdsManager, *statsRecorder_, *fileCache_, cmsSettings_.resourcesPath(), playerSettings_.displayName());

    interval->updateInterval(playerSettings_.collectInterval());
    playerSettings_.collectInterval().valueChanged().connect(
        std::bind(&CollectionInterval::updateInterval, interval.get(), ph::_1));

    interval->collectionFinished().connect(std::bind(&XiboApp::onCollectionFinished, this, ph::_1));
    interval->scheduleAvailable().connect(std::bind(&Scheduler::reloadSchedule, scheduler_.get(), ph::_1));
    interval->filesDownloaded().connect(std::bind(&Scheduler::reloadQueue, scheduler_.get()));
    interval->settingsUpdated().connect([this](const PlayerSettings& settings) {
        CHECK_UI_THREAD();
        playerSettings_.fromFields(settings);
        playerSettings_.saveTo(AppConfig::playerSettingsPath());
    });

    return interval;
}

std::unique_ptr<ScreenShotInterval> XiboApp::createScreenshotInterval(XmdsRequestSender& xmdsManager,
                                                                      Xibo::Window& window)
{
    auto interval = std::make_unique<ScreenShotInterval>(xmdsManager, window);

    interval->updateInterval(playerSettings_.screenshotInterval());
    playerSettings_.screenshotInterval().valueChanged().connect(
        std::bind(&ScreenShotInterval::updateInterval, interval.get(), ph::_1));

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
