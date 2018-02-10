#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"
#include "MainLayout.hpp"
#include "WebView.hpp"

#include "MainParser.hpp"
#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <gtkmm/application.h>
#include <boost/program_options.hpp>

#include <iostream>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

po::variables_map vm;

std::string get_xlf_path()
{
    auto&& root = vm["example-dir"].as<std::string>();
    if(!fs::exists(root) || !fs::is_directory(root)) return std::string{};

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator end;

    while(it != end)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ".xlf") return it->path().generic_string();
        ++it;
    }

    return std::string{};
}

int main(int argc, char *argv[])
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    auto logger = spdlog::get(LOGGER);
    auto app = Gtk::Application::create("org.gtkmm.xibo");

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("example-dir", po::value<std::string>(), "specify full (absolute) path to example directory");

    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if (vm.count("example-dir"))
    {
        logger->info("Example path is {}", vm["example-dir"].as<std::string>());
    }
    else
    {
        logger->info("Example was not chosen");
        return -1;
    }

    MainParser parser(get_xlf_path());

    auto layout = parser.parse();
    layout->show_regions();

    return app->run(*layout);
}
