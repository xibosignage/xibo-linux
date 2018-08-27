#include "XiboApp.hpp"
#include "config.hpp"
#include "utils/utilities.hpp"

#include "xmds/XMDSManager.hpp"
#include "control/MainLayout.hpp"
#include "control/MainWindow.hpp"
#include "tests/test.hpp"

#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <glibmm/main.h>
#include <gst/gst.h>
#include <boost/filesystem/operations.hpp>
#include <chrono>

std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    m_app = std::unique_ptr<XiboApp>(new XiboApp(name));
    return *m_app;
}

XiboApp::XiboApp(const std::string& name) : Gtk::Application(name)
{
    m_logger = spdlog::get(LOGGER);
}

int XiboApp::init_player()
{
    MainWindow window(500, 500, false, false, true, true);

    m_xmds_manager.reset(new XMDSManager{m_options.host(), m_options.server_key(), m_options.hardware_key()});

    signal_startup().connect([this, &window](){
        Gtk::Application::add_window(window);
    });

    auto run_player = std::bind(&XiboApp::run_player, this, std::ref(window));
    m_collection_interval.signal_finished().connect(run_player);

    auto update_settings = std::bind(&XiboApp::update_settings, this, std::placeholders::_1);
    m_collection_interval.signal_settings_updated().connect(update_settings);

    m_collection_interval.start();

    return Gtk::Application::run();
}

void XiboApp::run_player(MainWindow& window)
{
    if(!window.is_visible())
    {
        m_logger->info("Player started");
        m_layout = utils::parse_xlf_layout(find_xlf_file());
        window.add(*m_layout);
        window.show_all();
    }
}

void XiboApp::update_settings(const PlayerSettings& )
{
    //    spdlog::set_level(static_cast<spdlog::level::level_enum>(settings.log_level));
}

// FIXME temporary workaround
std::string XiboApp::find_xlf_file()
{
    namespace fs = boost::filesystem;

    fs::directory_iterator it(utils::resources_dir());
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

XMDSManager& XiboApp::xmds_manager()
{
    return *m_xmds_manager;
}

DownloadManager& XiboApp::download_manager()
{
    return m_download_manager;
}

int XiboApp::run(int argc, char** argv)
{
    try
    {
        m_options.parse(argc, argv);

        if(m_options.help_option())
        {
            m_logger->info("{}", m_options.available_options());
        }
        else
        {
            if(m_options.version_option())
            {
                m_logger->info("Project version: {}", get_version());
            }
            if(m_options.credentials())
            {
                return init_player();
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
