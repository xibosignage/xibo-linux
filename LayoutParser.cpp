#include "LayoutParser.hpp"
#include "RegionParser.hpp"
#include "utilities.hpp"

#include <iostream>

LayoutParser::LayoutParser(const boost::property_tree::ptree& _node) : Parser(_node)
{

}

Layout* LayoutParser::Parse()
{
    auto layout = InitObject();

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

Layout* LayoutParser::InitObject()
{
    std::cout << "parse layout" << std::endl;

    auto attrs = m_tree.get_child("<xmlattr>");

    Layout* layout = new Layout;
    layout->schemaVersion = attrs.get<int>("schemaVersion");
    layout->width = attrs.get<int>("width");
    layout->height = attrs.get<int>("height");
    layout->backgroundImage = attrs.get<std::string>("background");
    layout->backgroundColor = attrs.get<std::string>("bgcolor");

    return layout;
}
