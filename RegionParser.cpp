#include "RegionParser.hpp"
#include "MediaParser.hpp"
#include "utilities.hpp"

#include <iostream>

RegionParser::RegionParser(const boost::property_tree::ptree& tree) : Parser(tree)
{

}

Region* RegionParser::Parse()
{
    auto region = InitObject();

    for(auto&& pair : m_tree)
    {
        if(pair.first == "media")
        {
            auto media = std::shared_ptr<Media>(utilities::GetParser<MediaParser>(pair.second)->Parse());
            region->medias.push_back(media);
        }
    }

    return region;
}

Region* RegionParser::InitObject()
{
    std::cout << "parse region" << std::endl;

    auto attrs = m_tree.get_child("<xmlattr>");

    Region* region = new Region;
    region->id = attrs.get<int>("id");
    region->width = static_cast<int>(attrs.get<float>("width"));
    region->height = static_cast<int>(attrs.get<float>("height"));
    region->top = static_cast<int>(attrs.get<float>("top"));
    region->left = static_cast<int>(attrs.get<float>("left"));
    region->zindex = attrs.get<int>("zindex");
    region->loop = m_tree.get_child("options").get<bool>("loop", false);
    // transition add

    return region;
}
