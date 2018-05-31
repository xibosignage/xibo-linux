#include "LayoutParser.hpp"
#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "parsers/RegionParser.hpp"
#include <boost/property_tree/xml_parser.hpp>

LayoutParser::LayoutParser(const xlf_node& layout_node) :
    m_layout_node(layout_node)
{
}

std::unique_ptr<MainLayout> LayoutParser::parse()
{
    auto p = parse_params();
    auto layout = MainLayout::create(p.schemaVersion, p.width, p.height, p.bgimage, p.bgcolor);

    for(auto [layout_node_name, region_node] : m_layout_node)
    {
        if(layout_node_name == "region")
        {
            try
            {
                layout->add_region(RegionParser(region_node).parse());
            }
            catch(const boost::property_tree::ptree_bad_path& e)
            {
                spdlog::get(LOGGER)->error("[RegionParser]: Some regions won't be rendered - {}", e.what());
            }

        }
    }
    return layout;
}

boost::property_tree::ptree LayoutParser::get_layout_node(const std::string& xlf_path)
{
    boost::property_tree::ptree tree;
    boost::property_tree::read_xml(xlf_path, tree);
    return tree.get_child("layout");
}

LayoutParser::ParsedLayout LayoutParser::parse_params()
{
    spdlog::get(LOGGER)->debug("parse layout");
    auto attrs = m_layout_node.get_child("<xmlattr>");

    ParsedLayout params;
    params.schemaVersion = attrs.get<int>("schemaVersion");
    params.width = attrs.get<int>("width");
    params.height = attrs.get<int>("height");
    params.bgimage = attrs.get<std::string>("background", {});
    params.bgcolor = attrs.get<std::string>("bgcolor", {});
    return params;
}
