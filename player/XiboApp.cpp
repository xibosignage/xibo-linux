#include "XiboApp.hpp"
#include "MainLoop.hpp"
#include "config.hpp"

#include "utils/ScreenShoter.hpp"
#include "utils/Resources.hpp"
#include "xmlsink/XmlLoggerSink.hpp"

#include "control/MainWindowController.hpp"
#include "control/MainWindow.hpp"

#include "managers/CollectionInterval.hpp"
#include "managers/LayoutScheduler.hpp"
#include "managers/FileCacheManager.hpp"
#include "managers/PlayerSettingsManager.hpp"
#include "common/CmsSettingsManager.hpp"
#include "networking/HttpManager.hpp"
#include "networking/xmds/XmdsRequestSender.hpp"
#include "networking/xmds/SoapRequestSender.hpp"

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
    Resources::setDirectory(std::filesystem::current_path() / DEFAULT_RESOURCES_DIR);

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

XiboApp::XiboApp(const std::string& name) :
    m_mainLoop(std::make_unique<MainLoop>(name)),
    m_scheduler(std::make_unique<LayoutScheduler>()),
    m_fileManager(std::make_unique<FileCacheManager>()),
    m_playerSettingsManager(std::make_unique<PlayerSettingsManager>(DEFAULT_PLAYER_SETTINGS_FILE))
{
    if(!FileSystem::exists(DEFAULT_CMS_SETTINGS_FILE))
        throw std::runtime_error("Update CMS settings using player options app");

    CmsSettingsManager cmsSettingsManager{DEFAULT_CMS_SETTINGS_FILE};
    m_cmsSettings = cmsSettingsManager.load();
    if(!m_cmsSettings.resourcesPath.value().empty())
        Resources::setDirectory(FilePath{m_cmsSettings.resourcesPath});

    m_playerSettingsManager->load();
    m_fileManager->loadCache(Resources::directory() / DEFAULT_CACHE_FILE);
    HttpManager::instance().setProxyServer(m_cmsSettings.domain, m_cmsSettings.username, m_cmsSettings.password);

    m_mainLoop->setShutdownAction([this](){
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
    auto mainWindow = std::make_shared<MainWindow>(1366, 768);
    MainWindowController controller{mainWindow, *m_scheduler};

    m_screenShoter = std::make_unique<ScreenShoter>(*mainWindow);
    m_xmdsManager = std::make_unique<XmdsRequestSender>(m_cmsSettings.cmsAddress, m_cmsSettings.key, m_cmsSettings.displayId);
    m_collectionInterval = createCollectionInterval(*m_xmdsManager);

    updateSettings(m_playerSettingsManager->settings());

    m_collectionInterval->collectOnce([this, &controller, mainWindow](const PlayerError& error){

        onCollectionFinished(error);
        m_collectionInterval->startRegularCollection();

        controller.updateLayout(m_scheduler->nextLayoutId());
        mainWindow->showAll();

        Log::info("Player started");
    });

    return m_mainLoop->run(*mainWindow);
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
    Log::debug("Player settings updated");
}
