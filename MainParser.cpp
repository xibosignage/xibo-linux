#include "MainParser.hpp"
#include "RegionParser.hpp"
#include "utilities.hpp"

#include <iostream>

MainParser::MainParser(const std::string& file_name) :
    Parser<Layout>(boost::property_tree::ptree{})
{
    std::fstream file(file_name);

    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(file, tree);

    m_tree = tree.get_child("layout");
}

std::shared_ptr<Layout> MainParser::parse()
{
    auto layout = create_from_attrs(m_tree.get_child("<xmlattr>"));

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

std::shared_ptr<Layout> MainParser::create_from_attrs(const boost::property_tree::ptree& attrs)
{
    std::cout << "parse layout" << std::endl;

    int schemaVersion = attrs.get<int>("schemaVersion");
    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string backgroundImage = attrs.get<std::string>("background");
    std::string backgroundColor = attrs.get<std::string>("bgcolor");

    return std::make_shared<Layout>(schemaVersion, width, height, backgroundImage, backgroundColor);
}
