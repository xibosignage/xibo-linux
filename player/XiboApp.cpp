#include "XiboApp.hpp"
#include "config.hpp"
#include "utils/utilities.hpp"

#include "control/MainLayout.hpp"
#include "control/MainWindow.hpp"
#include "tests/test.hpp"

#include <spdlog/fmt/ostr.h>
#include <glibmm/main.h>
#include <gst/gst.h>

std::unique_ptr<XiboApp> XiboApp::m_app;

XiboApp& XiboApp::create(const std::string& name)
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    m_app = std::unique_ptr<XiboApp>(new XiboApp);
    m_app->m_parent_app = Gtk::Application::create(name);

    return *m_app;
}

XiboApp::XiboApp() :
    m_soap_manager{"linuxplayer.xibo.co.uk", 80}
{
    m_logger = spdlog::get(LOGGER);
}

XiboApp::~XiboApp()
{
    if(gst_is_initialized())
    {
        gst_deinit();
    }
}

const XiboApp& XiboApp::app()
{
    return *m_app;
}

const CommandLineParser& XiboApp::command_line_parser() const
{
    return m_options;
}

const SOAPManager& XiboApp::soap_manager() const
{
    return m_soap_manager;
}

int XiboApp::run(int argc, char** argv)
{    
    bool result = m_options.parse(argc, argv);
    if(result)
    {
        if(m_options.is_version())
        {
            m_logger->info("Project version: {}", get_version());
        }
        if(m_options.is_testing())
        {
            spdlog::set_level(spdlog::level::off);

            ::testing::InitGoogleTest(&argc, argv);
            return RUN_ALL_TESTS();
        }
        if(m_options.is_example_dir())
        {
            auto layout = utils::parse_xlf_layout(m_options.xlf_path());

            MainWindow window(500, 500, false, false, true, true);
            window.add(*layout);
            window.show_all();

            return m_parent_app->run(window);
        }
    }
    return 0;
}
