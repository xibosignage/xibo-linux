#include "XiboApp.hpp"
#include "config.hpp"
#include "constants.hpp"

#include "control/Region.hpp"
#include "control/MainLayout.hpp"
#include "creators/LayoutBuilder.hpp"

#include <spdlog/fmt/ostr.h>
#include <glibmm/main.h>

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

const XiboApp& XiboApp::app()
{
    return *m_app;
}

const XlfParser& XiboApp::xlf_parser() const
{
    return m_parser;
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
            m_parser.parse_xlf(m_options.xlf_file());

            auto layout = LayoutBuilder::create();
            layout->show_regions();

            m_logger->info("Player started");

            return m_parent_app->run(*layout);
        }
    }
    return 0;
}
