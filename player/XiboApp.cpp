#include "XiboApp.hpp"
#include "config.hpp"

#include "utils/XlfParser.hpp"
#include "utils/constants.hpp"
#include "utils/CommandLineParser.hpp"
#include "control/Region.hpp"
#include "control/MainLayout.hpp"
#include "creators/LayoutBuilder.hpp"

#include <spdlog/fmt/ostr.h>
#include <glibmm/main.h>

std::string XiboApp::s_example_dir;

XiboApp::XiboApp(const std::string& app_name)
{
    init();
    m_app = Gtk::Application::create(app_name);
}

void XiboApp::init()
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    m_logger = spdlog::get(LOGGER);
}

int XiboApp::run(int argc, char** argv)
{
    CommandLineParser options;
    bool result = options.parse(argc, argv);
    if(result)
    {
        if(options.is_version())
        {
            m_logger->info("Project version: {}", get_version());
        }
        if(options.is_example_dir())
        {
            if(options.is_testing())
            {
                Glib::signal_timeout().connect_once([=](){
                    std::exit(0);
                }, 3000);
            }
            s_example_dir = options.example_dir();

            XlfParser parser(options.xlf_file());
            parser.parse_xlf_tree();

            auto layout = LayoutBuilder::create(parser.get_layout_params());
            layout->show_regions();

            m_logger->info("Player started");

            return m_app->run(*layout);
        }
    }
    return 0;
}

// temporary solution until receiving files from CMS
const std::string& XiboApp::example_dir()
{
    return s_example_dir;
}
