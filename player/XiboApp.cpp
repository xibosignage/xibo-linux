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

int XiboApp::run_player()
{
    MainWindow window(500, 500, false, false, true, true);
    std::unique_ptr<MainLayout> layout;

    signal_startup().connect([this, &window](){
        Gtk::Application::add_window(window);
    });

    m_xmds_manager.reset(new XMDSManager{m_options.host(), m_options.server_key(), m_options.hardware_key()});
    m_collection_interval.signal_finished().connect([this, &layout, &window](){
        m_logger->info("Player started");
        layout = utils::parse_xlf_layout(utils::resources_dir() / "3.xlf");
        window.add(*layout);
        window.show_all();
    });
    m_collection_interval.start();

    return Gtk::Application::run();
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

const CommandLineParser& XiboApp::command_line_parser() const
{
    return m_options;
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
            if(m_options.log_level_option())
            {
                spdlog::set_level(static_cast<spdlog::level::level_enum>(m_options.log_level()));
            }
            if(m_options.credentials())
            {
                return run_player();
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
