#include "XiboApp.hpp"
#include "MainLoop.hpp"
#include "config.hpp"

#include "utils/Resources.hpp"
#include "utils/logger/XmlLoggerSink.hpp"
#include "parsers/CommandLineParser.hpp"
#include "events/CallbackEventQueue.hpp"

#include "controller/MainWindowController.hpp"
#include "view/MainWindow.hpp"

#include "managers/CollectionInterval.hpp"
#include "managers/LayoutScheduler.hpp"
#include "managers/FileCacheManager.hpp"
#include "managers/HttpManager.hpp"
#include "xmds/XmdsRequestSender.hpp"
#include "xmds/SoapRequestSender.hpp"

#include <gst/gst.h>
#include <glibmm/main.h>
#include <spdlog/sinks/stdout_sinks.h>

const std::string DEFAULT_RESOURCES_DIR = "resources";
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
    m_httpManager(std::make_unique<HttpManager>()),
    m_options(std::make_unique<CommandLineParser>())
{
    m_mainLoop->setIdleAction(std::bind(&XiboApp::processCallbackQueue, this));
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

    Log::logger()->setLevel(settings.logLevel);
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

HttpManager& XiboApp::httpManager()
{
    return *m_httpManager;
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
    try
    {
        auto window = std::make_shared<MainWindow>(1366, 768);
        MainWindowController mainController{window, *m_scheduler};

        m_xmdsManager.reset(new XmdsRequestSender{m_options->host(), m_options->serverKey(), m_options->hardwareKey()});
        m_collectionInterval = std::make_unique<CollectionInterval>(*m_xmdsManager);

        m_mainLoop->setShutdownAction([this](){
            m_httpManager->shutdown();
            m_collectionInterval->stop();
        });

        m_collectionInterval->subscribe(EventType::CollectionFinished, [this](const Event& ev){
            auto&& collectionEvent = static_cast<const CollectionFinishedEvent&>(ev);
            onCollectionFinished(collectionEvent.result());
        });

        m_collectionInterval->collectOnce([=, &mainController](const CollectionResult& result){
            onCollectionFinished(result);
            m_collectionInterval->startRegularCollection();

            mainController.updateLayout(m_scheduler->nextLayoutId());
            window->show();
            Log::info("Player started");
        });

        return m_mainLoop->run(*window);
    }
    catch(std::exception& e)
    {
        Log::error("Player failed to start: {}", e.what());
    }

    return -1;
}
