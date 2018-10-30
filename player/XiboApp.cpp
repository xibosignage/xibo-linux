#include "XiboApp.hpp"
#include "config.hpp"
#include "utils/Utilities.hpp"
#include "utils/Resources.hpp"

#include "xmds/XMDSManager.hpp"
#include "control/MainLayout.hpp"
#include "control/MainWindow.hpp"
#include "creators/MainBuilder.hpp"
#include "adaptors/GtkWindowAdaptor.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <glibmm/main.h>
#include <gst/gst.h>
#include <chrono>

const std::string DEFAULT_RESOURCES_DIR = "LayerTest";

std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    gst_init(nullptr, nullptr);
    Resources::setDirectory(std::filesystem::current_path() / DEFAULT_RESOURCES_DIR);

    m_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *m_app;
}

XiboApp::XiboApp(const std::string& name) : Gtk::Application(name)
{
    m_logger = spdlog::get(LOGGER);
}

int XiboApp::initPlayer()
{
    auto window = std::make_unique<MainWindow>(std::make_unique<GtkWindowAdaptor>());
    window->setSize(600, 600);

//    m_xmds_manager.reset(new XMDSManager{m_options.host(), m_options.server_key(), m_options.hardware_key()});

//    signal_startup().connect([this, &window](){
//        Gtk::Application::add_window(window);
//    });

//    auto run_player = std::bind(&XiboApp::run_player, this, std::ref(window));
//    m_collection_interval.signal_finished().connect(run_player);

//    auto update_settings = std::bind(&XiboApp::update_settings, this, std::placeholders::_1);
//    m_collection_interval.signal_settings_updated().connect(update_settings);

//    m_collection_interval.start();

    auto parsedXlfTree = Utils::parseXmlFromPath(findXlfFile());
    MainBuilder controller;
//    window->setFullscreen(true);
    window->addLayout(controller.buildLayoutWithChildren(parsedXlfTree));
    window->showLayout();

    return Gtk::Application::run(static_cast<GtkWindowAdaptor&>(window->handler()).get());
}

void XiboApp::runPlayer(MainWindow& window)
{
//    if(!window.isVisible())
//    {
//        m_logger->info("Player started");
//        m_layout = utils::parseAndCreateXlfLayout(findXlfFile());
//        window.addLayout(*m_layout);
//        window.showLayout();
//    }
}

void XiboApp::updateSettings(const PlayerSettings& )
{
//    spdlog::set_level(static_cast<spdlog::level::level_enum>(settings.log_level));
}

// FIXME temporary workaround
std::string XiboApp::findXlfFile()
{
    namespace fs = std::filesystem;

    fs::directory_iterator it(Resources::directory());
    fs::directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf")
            return it->path().string();
        ++it;
    }

    throw std::runtime_error(".XLF file was not found");
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

DownloadManager& XiboApp::downloadManager()
{
    return m_downloadManager;
}

int XiboApp::run(int argc, char** argv)
{
    try
    {
        m_options.parse(argc, argv);

        if(m_options.helpOption())
        {
            m_logger->info("{}", m_options.availableOptions());
        }
        else
        {
            if(m_options.versionOption())
            {
                m_logger->info("Project version: {}", getVersion());
            }
            if(m_options.credentials())
            {
                return initPlayer();
            }
        }
    }
    catch(std::exception& ex)
    {
        m_logger->error(ex.what());
        return -1;
    }
    return 0;
}
