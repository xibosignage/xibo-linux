#include "XiboApp.hpp"
#include "constants.hpp"

#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"
#include "MainLayout.hpp"
#include "WebView.hpp"

#include "XlfParser.hpp"

#include <iostream>
#include <gstreamermm/init.h>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;

std::string XiboApp::s_example_dir;

XiboApp::XiboApp(const std::string& app_name)
{
    init();
    m_app = Gtk::Application::create(app_name);
}

int XiboApp::run(int argc, char** argv)
{
    auto result = parse_command_line(argc, argv);

    if(result == ParsedResult::Success)
    {
        auto xlf_file_path = get_xlf_file();
        if(!xlf_file_path.empty())
        {
            XlfParser parser(xlf_file_path);

            auto layout = parser.parse_layout();
            layout->show_regions();

            return m_app->run(*layout);
        }
        else
        {
            m_logger->error(".XLF file doesn't exist");
        }
    }
    return ParsedResult::Failure;
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

int XiboApp::parse_command_line(int argc, char **argv)
{
    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()("example-dir", po::value<std::string>(), "specify full (absolute) path to example directory");

    po::store(po::parse_command_line(argc, argv, desc), m_vm);
    po::notify(m_vm);

    if (m_vm.count("example-dir"))
    {
        s_example_dir = m_vm["example-dir"].as<std::string>();

        if(!fs::exists(s_example_dir) || !fs::is_directory(s_example_dir))
        {
            m_logger->error("The directory doesn't exist (or it isn't a directory)");
            return ParsedResult::Failure;
        }

        m_logger->info("Example directory is {}", m_vm["example-dir"].as<std::string>());
    }
    else
    {
        m_logger->info("Example was not chosen");
        return ParsedResult::Failure;
    }

    return ParsedResult::Success;
}

std::string XiboApp::get_xlf_file()
{
    fs::recursive_directory_iterator it(s_example_dir);
    fs::recursive_directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf")
            return it->path().generic_string();
        ++it;
    }

    return std::string{};
}
