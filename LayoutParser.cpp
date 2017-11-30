#include "LayoutParser.hpp"
#include "RegionParser.hpp"
#include "utilities.hpp"

#include <iostream>

LayoutParser::LayoutParser(const boost::property_tree::ptree& _node) : Parser(_node)
{

}

std::shared_ptr<Layout> LayoutParser::Parse()
{
    auto layout = CreateObjectFromAttrs();

    for(auto&& pair : m_tree)
    {
        if(pair.first == "region")
        {
            auto region = utilities::GetParser<RegionParser>(pair.second)->Parse();
            layout->regions.push_back(region);
        }
    }


    return layout;
}

std::shared_ptr<Layout> LayoutParser::CreateObjectFromAttrs()
{
    std::cout << "parse layout" << std::endl;

    auto attrs = m_tree.get_child("<xmlattr>");

    int schemaVersion = attrs.get<int>("schemaVersion");
    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string backgroundImage = attrs.get<std::string>("background");
    std::string backgroundColor = attrs.get<std::string>("bgcolor");

    return std::make_shared<Layout>(schemaVersion, width, height, backgroundImage, backgroundColor);
}
