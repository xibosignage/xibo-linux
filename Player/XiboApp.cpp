#include "XiboApp.hpp"
#include "constants.hpp"
#include "config.hpp"

#include "XlfParser.hpp"
#include "XiboFactory.hpp"
#include "Region.hpp"
#include "MainLayout.hpp"

#include <iostream>
#include <gstreamermm/init.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>
#include <spdlog/fmt/ostr.h>
#include <glibmm/main.h>

namespace fs = boost::filesystem;

std::string XiboApp::s_example_dir;

XiboApp::XiboApp(const std::string& app_name)
{
    init();
    m_app = Gtk::Application::create(app_name);
}

int XiboApp::run(int argc, char** argv)
{
    parse_command_line(argc, argv);
    if(fs::exists(s_example_dir) && fs::is_directory(s_example_dir))
    {
        auto xlf_file_path = get_xlf_file();
        if(!xlf_file_path.empty())
        {
            XlfParser parser(xlf_file_path);
            parser.parse_xlf_tree();

            auto layout = XlfFactory::create_layout(parser.get_layout_params());
            layout->show_regions();

            m_logger->debug("Player started");

            return m_app->run(*layout);
        }
        else
        {
            m_logger->error(".XLF file doesn't exist");
        }
    }
    else if(!s_example_dir.empty())
    {
        m_logger->error("The directory doesn't exist (or it isn't a directory)");
    }
    return 0;
}

// temporary solution until receiving files from CMS
const std::string& XiboApp::example_dir()
{
    return s_example_dir;
}

void XiboApp::init()
{
    Gst::init();

    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    m_logger = spdlog::get(LOGGER);
}

void XiboApp::parse_command_line(int argc, char **argv)
{
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("example-dir", po::value<std::string>()->value_name("path-to-dir"), "specify full (absolute) path to example directory");
    desc.add_options()("version", "get project version");
    desc.add_options()("testing", "enable testing mode");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if(!vm.empty())
    {
        if(vm.count("example-dir"))
        {
            s_example_dir = vm["example-dir"].as<std::string>();
            m_logger->info("Example directory is {}", vm["example-dir"].as<std::string>());
        }
        if(vm.count("version"))
        {
            m_logger->info("Project version: {}", PROJECT_VERSION);
        }
        if(vm.count("testing"))
        {
            Glib::signal_timeout().connect_once([=](){
                std::exit(0);
            }, 3000);
        }
    }
    else
    {
        m_logger->info("{}", desc);
    }
}

std::string XiboApp::get_xlf_file()
{
    fs::directory_iterator it(s_example_dir);
    fs::directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf")
            return it->path().generic_string();
        ++it;
    }

    return std::string{};
}
