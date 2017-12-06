#include "RegionParser.hpp"
#include "MediaParser.hpp"
#include "ImageParser.hpp"
#include "VideoParser.hpp"
#include "utilities.hpp"

#include <spdlog/spdlog.h>

RegionParser::RegionParser(const boost::property_tree::ptree& tree) : Parser(tree)
{

}

std::shared_ptr<Region> RegionParser::parse()
{
    auto region = create_from_attrs(m_tree.get_child("<xmlattr>"));

    for(auto&& pair : m_tree)
    {
        if(pair.first == "media")
        {
            std::shared_ptr<Media> media;
            auto type = pair.second.get_child("<xmlattr>").get<std::string>("type");

            if(type == "image")
            {
                media = utilities::get_parser<ImageParser>(pair.second)->parse();
            }
            else if(type == "video")
            {
                media = utilities::get_parser<VideoParser>(pair.second)->parse();
            }

            region->add_media(media);
        }
    }

    return region;
}

std::shared_ptr<Region> RegionParser::create_from_attrs(const boost::property_tree::ptree& attrs)
{
    spdlog::get(LOGGER)->debug("parse region");

    auto options = m_tree.get_child("options");

    int id = attrs.get<int>("id");
    int width = static_cast<int>(attrs.get<float>("width"));
    int height = static_cast<int>(attrs.get<float>("height"));
    int top = static_cast<int>(attrs.get<float>("top"));
    int left = static_cast<int>(attrs.get<float>("left"));
    int zindex = attrs.get<int>("zindex");
    bool loop = options.get<bool>("loop", false);
    auto type = options.get<std::string>("transitionType",  std::string{});
    auto direction = options.get<std::string>("transitionDirection", std::string{});
    int duration = options.get<int>("transitionDuration", 0);

    return std::make_shared<Region>(id, Size{width, height}, Point{top, left}, zindex, loop, Transition{type, direction, duration});
}
