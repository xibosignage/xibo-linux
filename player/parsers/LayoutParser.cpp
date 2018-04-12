#include "LayoutParser.hpp"
#include "parsers/RegionParser.hpp"

#include <boost/property_tree/xml_parser.hpp>

LayoutParser::LayoutParser(const std::string& path)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(path, tree);
    m_layout_node = tree.get_child("layout");
}

ParsedLayout LayoutParser::parse_layout()
{    
    auto parsed_layout = parse_layout_params();

    for(auto [layout_node_name, region_node] : m_layout_node)
    {
        if(layout_node_name == "region")
        {
            auto current_region = RegionParser(region_node).parse_region();
            parsed_layout.regions.push_back(current_region);
        }
    }

    return parsed_layout;
}

ParsedLayout LayoutParser::parse_layout_params()
{
    spdlog::get(LOGGER)->debug("parse layout");
    auto attrs = m_layout_node.get_child("<xmlattr>");

    ParsedLayout parsed_layout;
    parsed_layout.schemaVersion = attrs.get<int>("schemaVersion");
    parsed_layout.width = attrs.get<int>("width");
    parsed_layout.height = attrs.get<int>("height");
    parsed_layout.bgimage = attrs.get_optional<std::string>("background");
    parsed_layout.bgcolor = attrs.get_optional<std::string>("bgcolor");

    return parsed_layout;
}
