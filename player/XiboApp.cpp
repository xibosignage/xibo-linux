#include "XiboApp.hpp"
#include "config.hpp"
#include "constants.hpp"

#include "control/Region.hpp"
#include "control/MainLayout.hpp"
#include "control/MainWindow.hpp"
#include "parsers/LayoutParser.hpp"
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

XiboApp::XiboApp()
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
            try
            {
                auto layout = LayoutParser(LayoutParser::get_layout_node(m_options.xlf_path())).parse();

                MainWindow window(500, 500, false, false, true, true);
                window.add(*layout);
                window.show_all();

                return m_parent_app->run(window);
            }
            catch(const boost::property_tree::ptree_bad_path& e)
            {
                m_logger->error("LayoutParser: {}", e.what());
            }

        }
    }
    return 0;
}
