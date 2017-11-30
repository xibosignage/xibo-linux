#include "RegionParser.hpp"
#include "MediaParser.hpp"
#include "utilities.hpp"

#include <iostream>

RegionParser::RegionParser(const boost::property_tree::ptree& tree) : Parser(tree)
{

}

std::shared_ptr<Region> RegionParser::Parse()
{
    auto region = CreateObjectFromAttrs();

    for(auto&& pair : m_tree)
    {
        if(pair.first == "media")
        {
            std::shared_ptr<Media> media;

            auto type = pair.second.get_child("<xmlattr>").get<std::string>("type");
            if(type == "image")
            {
                media = utilities::GetParser<MediaParser<Image>>(pair.second)->Parse();
            }
            else if(type == "video")
            {
                media = utilities::GetParser<MediaParser<Video>>(pair.second)->Parse();
            }
            else if(type == "audio")
            {
                media = utilities::GetParser<MediaParser<Image>>(pair.second)->Parse();
            }
            region->medias.push_back(media);
        }
    }

    return region;
}

std::shared_ptr<Region> RegionParser::CreateObjectFromAttrs()
{
    std::cout << "parse region" << std::endl;

    auto attrs = m_tree.get_child("<xmlattr>");

    int id = attrs.get<int>("id");
    int width = static_cast<int>(attrs.get<float>("width"));
    int height = static_cast<int>(attrs.get<float>("height"));
    int top = static_cast<int>(attrs.get<float>("top"));
    int left = static_cast<int>(attrs.get<float>("left"));
    int zindex = attrs.get<int>("zindex");
    bool loop = m_tree.get_child("options").get<bool>("loop", false);
    // transition add

    return std::make_shared<Region>(id, width, height, top, left, zindex, loop);
}
