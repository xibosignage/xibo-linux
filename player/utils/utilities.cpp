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

DownloadManager& utils::downloadManager()
{
    return XiboApp::app().downloadManager();
}

XMDSManager& utils::xmdsManager()
{
    return XiboApp::app().xmdsManager();
}

boost::filesystem::path utils::resourcesDir()
{
    return downloadManager().resourcesDir();
}

std::unique_ptr<MediaParser> utils::getMediaParser(const xlf_node& parent_node, const xlf_node& media_node)
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

std::unique_ptr<MainLayout> utils::parseXlfLayout(const boost::filesystem::path& xlfPath)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlfPath.string(), tree);

    LayoutParser layout_parser(tree.get_child("layout"));
    auto layout = layout_parser.parse();
    for(auto regionNode : layout_parser)
    {
        RegionParser regionParser(regionNode);
        auto region = regionParser.parse();
        for(auto mediaNode : regionParser)
        {
            auto mediaParser = utils::getMediaParser(regionNode, mediaNode);
            region->addMedia(mediaParser->parse());
        }
        layout->addRegion(std::move(region));
    }
    return layout;
}

boost::property_tree::ptree utils::parseXml(const std::string& xml)
{
    std::stringstream stream;
    stream << xml;
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(stream, tree);
    return tree;
}
