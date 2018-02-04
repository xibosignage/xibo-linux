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


/* temporary workaround */
void copy_all_files(const std::string& path)
{
    auto logger = spdlog::get(LOGGER);

    if(!boost::filesystem::exists(path)) throw std::runtime_error("Not found");

    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(path + "/requiredFiles.xml", tree);

    auto root = tree.get_child("RequiredFiles").get_child("RequiredFileList");

    for(auto&& node : root)
    {
        auto file_name = node.second.get<std::string>("SaveAs");
        if(!boost::filesystem::exists("MediaFiles/" + file_name))
        {
            boost::filesystem::copy_file(path + "/" + file_name, "MediaFiles/" + file_name);
            logger->debug("Copied {}", file_name);
        }
    }

    if(boost::filesystem::exists("MediaFiles/requiredFiles.xml"))
        boost::filesystem::remove("MediaFiles/requiredFiles.xml");

    boost::filesystem::copy_file(path + "/requiredFiles.xml", "MediaFiles/requiredFiles.xml");
    logger->debug(path + "/requiredFiles.xml");
}

int main(int argc, char *argv[])
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    auto logger = spdlog::get(LOGGER);
    auto app = Gtk::Application::create("org.gtkmm.xibo");

    namespace po = boost::program_options;

    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "produce help message")
        ("example-dir", po::value<std::string>(), "specify path to example directory");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help"))
    {
        std::cout << desc << std::endl;
        return 1;
    }

    if(!boost::filesystem::exists("MediaFiles"))
        boost::filesystem::create_directory("MediaFiles");

    if (vm.count("example-dir"))
    {
        logger->info("Example path is {}", vm["example-dir"].as<std::string>());
        copy_all_files(vm["example-dir"].as<std::string>());
    }
    else
    {
        logger->info("Example was not chosen");
        return -1;
    }

    MainParser parser("MediaFiles/90.xlf");
    auto layout = parser.parse();
//    MainLayout* layout = new MainLayout(0, 1366, 768, "", "#000");

//    layout->add_region(0, Size{300, 300}, Point{0, 0}, 0, false, Transition{});
//    layout->region(0).add_media<Video>(0, 0, false, "/home/stivius/video.webm", false, false);

//    layout->add_region(1, Size{360, 360}, Point{0, 0}, 1, false, Transition{});
//    layout->region(1).add_media<WebView>(0, 0, false, "482.htm", 0, true);

    layout->show_regions();

    return app->run(*layout);
}
