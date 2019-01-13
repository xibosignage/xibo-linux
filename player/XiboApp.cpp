#include "XiboApp.hpp"
#include "config.hpp"

#include "utils/Resources.hpp"
#include "events/CallbackGlobalQueue.hpp"

#include "xmds/XMDSManager.hpp"
#include "xmds/SOAPManager.hpp"
#include "adaptors/GtkWindowAdaptor.hpp"
#include "control/MainWindow.hpp"
#include "control/IMainLayout.hpp"

#include "parsers/CommandLineParser.hpp"
#include "managers/HTTPDownloader.hpp"
#include "managers/CollectionInterval.hpp"
#include "managers/Scheduler.hpp"

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

    auto path = std::filesystem::current_path() / DEFAULT_RESOURCES_DIR;
//    Resources::removeDirectoryContents(path);
    Resources::setDirectory(path);

    m_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *m_app;
}

XiboApp::XiboApp(const std::string& name) :
    m_parentApp(Gtk::Application::create(name)),
    m_downloadManager(std::make_unique<HTTPDownloader>()),
    m_scheduler(std::make_unique<Scheduler>()),
    m_collectionInterval(std::make_unique<CollectionInterval>()),
    m_options(std::make_unique<CommandLineParser>())
{
    m_idleConnection = Glib::MainContext::get_default()->signal_idle().connect([this]{
        return processCallbackQueue();
    });

    m_scheduler->subscribe(EventType::LayoutExpired, [this](const Event&){
        m_mainWindow->setLayout(m_scheduler->nextLayout());
        m_mainWindow->showLayout();
    });
}
void XiboApp::updateSettings(const PlayerSettings& )
{
//    spdlog::set_level(static_cast<spdlog::level::level_enum>(settings.log_level));
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

HTTPDownloader& XiboApp::downloadManager()
{
    return *m_downloadManager;
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
        if(m_options->exampleDir())
        {
            Resources::setDirectory(m_options->getExampleDir());
        }
        if(m_options->credentials() || m_options->exampleDir())
        {
            return initMainLoop();
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

int XiboApp::initMainLoop()
{
    m_mainWindow = std::make_unique<MainWindow>(std::make_unique<GtkWindowAdaptor>());

    m_parentApp->signal_shutdown().connect([this](){
        m_idleConnection.disconnect();
    });

    m_parentApp->signal_startup().connect([this](){
        auto&& windowHandler = static_cast<GtkWindowAdaptor&>(m_mainWindow->handler()).get();
        m_parentApp->add_window(windowHandler);
    });

    if(m_options->credentials())
    {
        m_xmdsManager.reset(new XMDSManager{m_options->host(), m_options->serverKey(), m_options->hardwareKey()});
        m_collectionInterval->collect([this](const CollectionResult& result){
            m_scheduler->update(result.schedule);
            m_collectionInterval->start();
            runPlayer();
        });
    }
    else
    {
        runPlayer();
    }

    return m_parentApp->run();
}

void XiboApp::runPlayer()
{
    try
    {
        m_mainWindow->setSize(1366, 768);
//        m_mainWindow->setFullscreen(true);
        m_mainWindow->setLayout(m_scheduler->nextLayout());
        m_mainWindow->showLayout();
        m_mainWindow->show();

        Log::info("Player started");
    }
    catch(std::exception& e)
    {
        Log::error("Player failed to start: {}", e.what());
    }
}
