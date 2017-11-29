#include "RegionParser.hpp"
#include "MediaParser.hpp"
#include "utilities.hpp"

#include <iostream>

RegionParser::RegionParser(const boost::property_tree::ptree& tree) : Parser(tree)
{

}

Region* RegionParser::Parse()
{
    auto region = GetAttributes(m_tree.get_child("<xmlattr>"));

    for(auto&& pair : m_tree)
    {
        if(pair.first == "media")
        {
            auto media = std::shared_ptr<Media>(utilities::GetParser<MediaParser>(pair.second)->Parse());
            region->medias.push_back(media);
        }
        else if(pair.first == "options")
        {
            std::map<std::string, std::string> options = ParseOptions(pair.second);
            region->InitOptions(options);
        }
    }

    return region;
}

Region* RegionParser::GetAttributes(const boost::property_tree::ptree& tree)
{
    std::cout << "parse region" << std::endl;
    Region* region = new Region;
    region->id = tree.get<int>("id");
    region->width = static_cast<int>(tree.get<float>("width"));
    region->height = static_cast<int>(tree.get<float>("height"));
    region->top = static_cast<int>(tree.get<float>("top"));
    region->left = static_cast<int>(tree.get<float>("left"));
    region->zindex = tree.get<int>("zindex");

    return region;
}
