#include "XiboApp.hpp"
#include "config.hpp"
#include "constants.hpp"

#include "control/Region.hpp"
#include "control/MainLayout.hpp"
#include "creators/LayoutBuilder.hpp"
#include "parsers/LayoutParser.hpp"

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
        if(m_options.is_example_dir())
        {
            try
            {
                LayoutParser parser(m_options.xlf_file());

                auto layout = LayoutBuilder(parser.parse_layout()).build();
                layout->show_regions();

                m_logger->info("Player started");

                return m_parent_app->run(*layout);
            }
            catch(const boost::property_tree::ptree_bad_path& e)
            {
                m_logger->error("LayoutParser: {}", e.what());
            }

        }
    }
    return 0;
}
