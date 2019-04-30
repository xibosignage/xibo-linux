#include "XiboApp.hpp"

#include "MainLoop.hpp"
#include "config.hpp"

#include "utils/ScreenShoter.hpp"
#include "utils/Resources.hpp"
#include "xmlsink/XmlLoggerSink.hpp"

#include "control/MainWindowController.hpp"
#include "control/MainWindow.hpp"
#include "control/media/player/video/XiboVideoSink.hpp"

#include "managers/CollectionInterval.hpp"
#include "managers/LayoutScheduler.hpp"
#include "managers/FileCacheManager.hpp"
#include "managers/PlayerSettingsManager.hpp"
#include "managers/XmrManager.hpp"

#include "networking/HttpManager.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"
#include "networking/xmds/SoapRequestSender.hpp"

#include "common/CmsSettingsManager.hpp"
#include "common/RsaManager.hpp"

#include <gst/gst.h>
#include <glibmm/main.h>
#include <spdlog/sinks/stdout_sinks.h>

std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    auto logger = Logger::create(LOGGER, createLoggerSinks());
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
    m_scheduler(std::make_unique<LayoutScheduler>()),
    m_fileManager(std::make_unique<FileCacheManager>()),
    m_playerSettingsManager(std::make_unique<PlayerSettingsManager>(ProjectResources::playerSettings())),
    m_xmrManager(std::make_unique<XmrManager>())
{
    if(!FileSystem::exists(ProjectResources::cmsSettings()))
        throw std::runtime_error("Update CMS settings using player options app");

    m_xmrManager->collectionInterval().connect([this](){
        Log::info("Start unscheduled collection");
        m_collectionInterval->collectOnce([this](const PlayerError& error){
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

    CmsSettingsManager cmsSettingsManager{ProjectResources::cmsSettings()};
    m_cmsSettings = cmsSettingsManager.load();
    if(!m_cmsSettings.resourcesPath.value().empty())
        Resources::setDirectory(FilePath{m_cmsSettings.resourcesPath});

    m_playerSettingsManager->load();
    m_fileManager->loadCache(Resources::resDirectory() / DEFAULT_CACHE_FILE);
    HttpManager::instance().setProxyServer(m_cmsSettings.domain, m_cmsSettings.username, m_cmsSettings.password);
    RsaManager::instance().load();

    m_mainLoop->setShutdownAction([this](){
        m_windowController.reset();
        m_xmrManager->stop();
        HttpManager::instance().shutdown();
        if(m_collectionInterval)
        {
            m_collectionInterval->stop();
        }
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

int XiboApp::run()
{
    auto mainWindow = std::make_shared<MainWindow>();
    m_windowController = std::make_unique<MainWindowController>(mainWindow, *m_scheduler);

    m_screenShoter = std::make_unique<ScreenShoter>(*mainWindow);
    m_xmdsManager = std::make_unique<XmdsRequestSender>(m_cmsSettings.cmsAddress, m_cmsSettings.key, m_cmsSettings.displayId);
    m_collectionInterval = createCollectionInterval(*m_xmdsManager);

    updateSettings(m_playerSettingsManager->settings());

    m_collectionInterval->collectOnce([this, mainWindow](const PlayerError& error){
        if(error)
        {
            onCollectionFinished(error);
            m_mainLoop->quit();
        }
        else
        {
            tryStartPlayer(mainWindow);
        }
    });

    return m_mainLoop->run(*mainWindow);
}

void XiboApp::tryStartPlayer(const std::shared_ptr<MainWindow>& window)
{
    int layoutId = m_scheduler->nextLayoutId();
    if(layoutId != 0)
    {
        m_windowController->updateLayout(layoutId);

        m_collectionInterval->startRegularCollection();
        window->show();

        Log::info("Player started");
    }
    else
    {
        Log::error("Scheduled and default layout have not been set");
        m_mainLoop->quit();
    }
}


std::unique_ptr<CollectionInterval> XiboApp::createCollectionInterval(XmdsRequestSender& xmdsManager)
{
    auto interval = std::make_unique<CollectionInterval>(xmdsManager);

    interval->collectionFinished().connect([this](const PlayerError& error){
        onCollectionFinished(error);
    });

    interval->settingsUpdated().connect([this](const PlayerSettings& settings){
        updateSettings(settings);
    });

    interval->scheduleUpdated().connect([this](const LayoutSchedule& schedule){
        m_scheduler->update(schedule);
    });

    return interval;
}

void XiboApp::onCollectionFinished(const PlayerError& error)
{
    if(error)
    {
        Log::debug("Collection interval error: {}", error);
    }
}

void XiboApp::updateSettings(const PlayerSettings& settings)
{
    m_playerSettingsManager->update(settings);
    applyPlayerSettings(settings);
}

void XiboApp::applyPlayerSettings(const PlayerSettings& settings)
{
    Log::logger()->setLevel(settings.logLevel);
    m_collectionInterval->updateInterval(settings.collectInterval);
    m_xmrManager->connect(settings.xmrNetworkAddress);
    m_windowController->updateWindowDimensions(settings.dimensions);

    Log::debug("Player settings updated");
}
