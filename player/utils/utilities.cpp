#include "utilities.hpp"
#include "constants.hpp"
#include "XiboApp.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"

#include "parsers/LayoutParser.hpp"
#include "parsers/RegionParser.hpp"
#include "parsers/ImageParser.hpp"
#include "parsers/VideoParser.hpp"
#include "parsers/AudioParser.hpp"
#include "parsers/WebViewParser.hpp"

#include <stdexcept>
#include <spdlog/spdlog.h>
#include <boost/filesystem/operations.hpp>
#include <boost/property_tree/xml_parser.hpp>

uint32_t utils::to_hex(const std::string& str_color)
{
    // remove '#' sign at the beginning
    std::string str_hex = str_color.substr(1);

    // convert 3-digit hex to 6-digit hex
    if(str_hex.size() == 3)
        str_hex = std::string(2, str_hex[0]) + std::string(2, str_hex[1]) + std::string(2, str_hex[2]);
    // add default alpha channel
    if(str_hex.size() == 6)
        str_hex += "FF";

    return static_cast<uint32_t>(std::stoul(str_hex, nullptr, 16));
}

XMDSManager& utils::xmds_manager()
{
    return XiboApp::app().xmds_manager();
}

const std::string& utils::example_dir()
{
    //return XiboApp::app().command_line_parser().example_dir_path();
}

std::string utils::app_current_dir()
{
    return boost::filesystem::current_path().string();
}

std::unique_ptr<MediaParser> utils::get_media_parser(const xlf_node& parent_node, const xlf_node& media_node)
{
    auto type = media_node.get_child("<xmlattr>").get<std::string>("type");

    if(type == "image")
        return std::make_unique<ImageParser>(parent_node, media_node);
    else if(type == "video")
        return std::make_unique<VideoParser>(parent_node, media_node);
    else if(type == "audio")
        return std::make_unique<AudioParser>(parent_node, media_node);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        return std::make_unique<WebViewParser>(parent_node, media_node);
}

std::unique_ptr<MainLayout> utils::parse_xlf_layout(const std::string& xlf_path)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlf_path, tree);

    LayoutParser layout_parser(tree.get_child("layout"));
    auto layout = layout_parser.parse();
    for(auto region_node : layout_parser)
    {
        RegionParser region_parser(region_node);
        auto region = region_parser.parse();
        for(auto media_node : region_parser)
        {
            auto media_parser = utils::get_media_parser(region_node, media_node);
            region->add_media(media_parser->parse());
        }
        layout->add_region(std::move(region));
    }
    return layout;
}
