#include "LayoutParser.hpp"
#include "RegionParser.hpp"
#include "utilities.hpp"

#include <iostream>

LayoutParser::LayoutParser(const boost::property_tree::ptree& _node) : Parser(_node)
{

}

Layout* LayoutParser::Parse()
{
    auto layout = GetAttributes(m_tree.get_child("<xmlattr>"));

    for(auto&& pair : m_tree)
    {
        if(pair.first == "region")
        {
            auto region = std::shared_ptr<Region>(utilities::GetParser<RegionParser>(pair.second)->Parse());
            layout->regions.push_back(region);
        }
    }

    return layout;
}

Layout* LayoutParser::GetAttributes(const boost::property_tree::ptree& tree)
{
    std::cout << "parse layout" << std::endl;
    Layout* layout = new Layout;
    layout->schemaVersion = tree.get<int>("schemaVersion");
    layout->width = tree.get<int>("width");
    layout->height = tree.get<int>("height");
    layout->backgroundImage = tree.get<std::string>("background");
    layout->backgroundColor = tree.get<std::string>("bgcolor");

    return layout;
}
