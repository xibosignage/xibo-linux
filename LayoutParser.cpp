#include "LayoutParser.hpp"
#include "RegionParser.hpp"
#include "utilities.hpp"

#include <iostream>

std::shared_ptr<Layout> LayoutParser::parse()
{
    auto layout = create_from_attrs();

    for(auto&& pair : m_tree)
    {
        if(pair.first == "region")
        {
            auto region = utilities::get_parser<RegionParser>(pair.second)->parse();
            layout->add_region(region);
        }
    }


    return layout;
}

std::shared_ptr<Layout> LayoutParser::create_from_attrs()
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
