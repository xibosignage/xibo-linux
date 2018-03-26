#include "XiboApp.hpp"
#include "constants.hpp"

#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"
#include "MainLayout.hpp"
#include "WebView.hpp"
#include "config.hpp"

#include "XlfParser.hpp"
#include "CommandLineParser.hpp"

#include <iostream>
#include <gstreamermm/init.h>
#include <glibmm/main.h>

std::string XiboApp::s_example_dir;

XiboApp::XiboApp(const std::string& app_name)
{
    init();
    m_app = Gtk::Application::create(app_name);
}

void XiboApp::init()
{
    Gst::init();

    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    m_logger = spdlog::get(LOGGER);
}

int XiboApp::run(int argc, char** argv)
{
    CommandLineParser options(argc, argv);
    bool result = options.parse();
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
            auto layout = parser.parse_layout();
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
