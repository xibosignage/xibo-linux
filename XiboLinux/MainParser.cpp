#include "MainParser.hpp"
#include "RegionParser.hpp"
#include "ImageParser.hpp"
#include "VideoParser.hpp"
#include "WebViewParser.hpp"

#include "Image.hpp"
#include "Video.hpp"
#include "WebView.hpp"

#include <spdlog/spdlog.h>

MainParser::MainParser(const std::string& full_path)
{
    spdlog::get(LOGGER)->debug(full_path);

    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(full_path, tree);

    m_tree = tree.get_child("layout");
}

std::unique_ptr<MainLayout> MainParser::parse()
{
    spdlog::get(LOGGER)->debug("parse layout");

    auto attrs = m_tree.get_child("<xmlattr>");

    int schemaVersion = attrs.get<int>("schemaVersion");
    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string backgroundImage = attrs.get_optional<std::string>("background").value_or("-");
    std::string backgroundColor = attrs.get_optional<std::string>("bgcolor").value_or("-");

    m_layout = std::make_unique<MainLayout>(schemaVersion, width, height, backgroundImage, backgroundColor);

    parse_xml_tree();

    return std::move(m_layout);
}

void MainParser::parse_xml_tree()
{
    for(auto&& layout : m_tree)
    {
        if(layout.first == "region")
        {
            auto region_node = layout.second;
            auto region = RegionParser(region_node).parse();
            add_region(std::make_index_sequence<std::tuple_size<ParsedRegion>::value>{}, region);

            for(auto&& region : region_node)
            {
                if(region.first == "media")
                {
                    auto type = region.second.get_child("<xmlattr>").get<std::string>("type");
                    if(type == "image")
                    {
                        auto image = ImageParser(region.second).parse();
                        add_media<Image>(std::make_index_sequence<std::tuple_size<ParsedImage>::value>{}, image);
                    }
                    else if(type == "video")
                    {
                        auto video = VideoParser(region.second).parse();
                        add_media<Video>(std::make_index_sequence<std::tuple_size<ParsedVideo>::value>{}, video);
                    }
                    else if(type == "twitter" || type == "forecastio" || type == "ticker")
                    {
                        auto web = WebViewParser(region.second).parse();
                        add_media<WebView>(std::make_index_sequence<std::tuple_size<ParsedWebView>::value>{}, web);
                    }
                }
            }
        }
    }
}
