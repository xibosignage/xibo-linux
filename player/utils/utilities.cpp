#include "utilities.hpp"
#include "constants.hpp"
#include "XiboApp.hpp"

#include "control/MainLayout.hpp"
#include "control/Region.hpp"

#include "factories/MainLayoutFactory.hpp"
#include "factories/RegionFactory.hpp"
#include "factories/ImageFactory.hpp"
#include "factories/VideoFactory.hpp"
#include "factories/AudioFactory.hpp"
#include "factories/WebViewFactory.hpp"

#include <stdexcept>
#include <boost/property_tree/xml_parser.hpp>

std::shared_ptr<spdlog::logger> utils::logger()
{
    static auto logger = spdlog::get(LOGGER);
    return logger;
}

DownloadManager& utils::downloadManager()
{
    return XiboApp::app().downloadManager();
}

XMDSManager& utils::xmdsManager()
{
    return XiboApp::app().xmdsManager();
}

std::filesystem::path utils::resourcesDir()
{
    return downloadManager().resourcesDir();
}

std::unique_ptr<MediaFactory> utils::getMediaFactory(const xlf_node& parentNode, const xlf_node& mediaNode)
{
    auto type = mediaNode.get_child("<xmlattr>").get<std::string>("type");

    if(type == "image")
        return std::make_unique<ImageFactory>(parentNode, mediaNode);
    else if(type == "video")
        return std::make_unique<VideoFactory>(parentNode, mediaNode);
    else if(type == "audio")
        return std::make_unique<AudioFactory>(parentNode, mediaNode);
    else // NOTE DataSetView, Embedded, Text and Ticker can be rendered via webview
        return std::make_unique<WebViewFactory>(parentNode, mediaNode);
}

std::unique_ptr<IMainLayout> utils::parseAndCreateXlfLayout(const std::filesystem::path& xlfPath)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlfPath.string(), tree);
    auto layoutNode = tree.get_child("layout");

    MainLayoutFactory layoutFactory(layoutNode);
    auto layout = layoutFactory.create();
    for(auto [nodeName, regionNode] : layoutNode)
    {
        if(nodeName == "region")
        {
            RegionFactory regionFactory(regionNode);
            auto region = regionFactory.create();
            for(auto [nodeName, mediaNode] : regionNode)
            {
                if(nodeName == "media")
                {
                    auto mediaFactory = utils::getMediaFactory(regionNode, mediaNode);
                    region->addMedia(mediaFactory->create());
                }
            }
            layout->addRegion(std::move(region));
        }
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
