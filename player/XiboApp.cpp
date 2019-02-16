#include "XiboApp.hpp"
#include "MainLoop.hpp"
#include "config.hpp"

#include "utils/Resources.hpp"
#include "events/CallbackGlobalQueue.hpp"

#include "xmds/XMDSManager.hpp"
#include "xmds/SOAPManager.hpp"
#include "adaptors/GtkWindowAdaptor.hpp"
#include "control/MainWindow.hpp"
#include "control/IMainLayout.hpp"

#include "parsers/CommandLineParser.hpp"
#include "managers/HTTPManager.hpp"
#include "managers/CollectionInterval.hpp"
#include "managers/Scheduler.hpp"
#include "managers/FileCacheManager.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <gst/gst.h>
#include <glibmm/main.h>

const std::string DEFAULT_RESOURCES_DIR = "resources";
std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S.%e] [%l]: %v");

    gst_init(nullptr, nullptr);
    Resources::setDirectory(std::filesystem::current_path() / DEFAULT_RESOURCES_DIR);

    m_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *m_app;
}

XiboApp::XiboApp(const std::string& name) :
    m_mainLoop(std::make_unique<MainLoop>(name)),
    m_downloadManager(std::make_unique<HTTPManager>()),
    m_scheduler(std::make_unique<Scheduler>()),
    m_fileManager(std::make_unique<FileCacheManager>()),
    m_collectionInterval(std::make_unique<CollectionInterval>()),
    m_options(std::make_unique<CommandLineParser>())
{
    m_mainLoop->setIdleAction(std::bind(&XiboApp::processCallbackQueue, this));

    m_collectionInterval->subscribe(EventType::CollectionFinished, [this](const Event& ev){
        auto&& collectionEvent = static_cast<const CollectionFinished&>(ev);
        onCollectionFinished(collectionEvent.result());
    });
}

void XiboApp::onCollectionFinished(const CollectionResult& result)
{
    if(!result.error)
    {
        Log::debug("Received collection result");

        m_scheduler->update(result.schedule);
        updateSettings(result.settings);
    }
    else
    {
        Log::debug("Collection interval error: {}", result.error);
    }
}

void XiboApp::updateSettings(const PlayerSettings& settings)
{
    Log::debug("Log level updated");

    spdlog::set_level(settings.logLevel);
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

XMDSManager& XiboApp::xmdsManager()
{
    return *m_xmdsManager;
}

HTTPManager& XiboApp::downloadManager()
{
    return *m_downloadManager;
}

FileCacheManager& XiboApp::fileManager()
{
    return *m_fileManager;
}

int XiboApp::run(int argc, char** argv)
{
    tryParseCommandLine(argc, argv);

    if(m_options->helpOption())
    {
        Log::info("{}", m_options->availableOptions());
    }
    else
    {
        if(m_options->versionOption())
        {
            Log::info("Project version: {}", getVersion());
        }
        if(m_options->credentials())
        {
            return runMainLoop();
        }
    }
    return 0;
}

void XiboApp::tryParseCommandLine(int argc, char** argv)
{
    try
    {
        m_options->parse(argc, argv);
    }
    catch(std::exception& e)
    {
        Log::error(e.what());
    }
}

bool XiboApp::processCallbackQueue()
{
    if(!callbackQueue().empty())
    {
        auto callback = callbackQueue().pop();
        callback();
    }
    return true;
}

int XiboApp::runMainLoop()
{
    auto mainWindow = std::make_shared<MainWindow>(std::make_unique<GtkWindowAdaptor>());

    m_xmdsManager.reset(new XMDSManager{m_options->host(), m_options->serverKey(), m_options->hardwareKey()});

    m_scheduler->subscribe(EventType::LayoutExpired, [this, &mainWindow](const Event&){
        mainWindow->setLayout(m_scheduler->nextLayout());
        mainWindow->showLayout();
    });

    m_collectionInterval->collectOnce([this, &mainWindow](const CollectionResult& result){
        onCollectionFinished(result);
        m_collectionInterval->startRegularCollection();
        startWindow(*mainWindow);
    });

    return m_mainLoop->run(mainWindow->handler());
}

void XiboApp::startWindow(MainWindow& window)
{
    try
    {
        window.setSize(1366, 768);
//        window.setFullscreen(true);
        window.setLayout(m_scheduler->nextLayout());
        window.showLayout();
        window.show();

        Log::info("Player started");
    }
    catch(std::exception& e)
    {
        Log::error("Player failed to start: {}", e.what());
    }
}
