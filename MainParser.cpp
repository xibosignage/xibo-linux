#include "MainParser.hpp"
#include "RegionParser.hpp"
#include "ImageParser.hpp"
#include "VideoParser.hpp"

#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"
#include "Video.hpp"

#include <spdlog/spdlog.h>

MainParser::MainParser(const std::string& file_name)
{
    std::fstream file(file_name);

    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(file, tree);

    m_tree = tree.get_child("layout");

    parse();
}

std::unique_ptr<MainLayout> MainParser::parse()
{
    spdlog::get(LOGGER)->debug("parse layout");

    auto attrs = m_tree.get_child("<xmlattr>");

    int schemaVersion = attrs.get<int>("schemaVersion");
    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string backgroundImage = attrs.get<std::string>("background");
    std::string backgroundColor = attrs.get<std::string>("bgcolor");

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
            m_layout->add_region(region.id, region.size, region.pos, region.zindex, region.looped, region.transition);

            for(auto&& region : region_node)
            {
                if(region.first == "media")
                {
                    auto type = region.second.get_child("<xmlattr>").get<std::string>("type");
                    if(type == "image")
                    {
                        auto image = ImageParser(region.second).parse();
                        m_layout->region(m_layout->regions_count() - 1).add_media<Image>(image.id, image.duration, image.uri, image.scale_type, image.align, image.valign);
                    }
                    else if(type == "video")
                    {
                        auto video = VideoParser(region.second).parse();
                        m_layout->region(m_layout->regions_count() - 1).add_media<Video>(video.id, video.duration, video.uri, video.muted, video.looped);
                    }
                }
            }
        }
    }
}
