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

DownloadManager& utils::download_manager()
{
    return XiboApp::app().download_manager();
}

XMDSManager& utils::xmds_manager()
{
    return XiboApp::app().xmds_manager();
}

boost::filesystem::path utils::resources_dir()
{
    return download_manager().resources_dir();
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

std::unique_ptr<MainLayout> utils::parse_xlf_layout(const boost::filesystem::path& xlf_path)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlf_path.string(), tree);

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

boost::property_tree::ptree utils::parse_xml(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}
