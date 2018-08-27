#include "LayoutParser.hpp"
#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "parsers/RegionParser.hpp"

LayoutParser::LayoutParser(const xlf_node& layout_node) :
    m_layout_node(layout_node)
{
    for(auto [layout_node_name, region_node] : m_layout_node)
    {
        if(layout_node_name == "region")
        {
            m_region_nodes.push_back(region_node);
        }
    }
}

std::unique_ptr<MainLayout> LayoutParser::parse()
{
    spdlog::get(LOGGER)->trace("Parsing layout...");
    auto attrs = m_layout_node.get_child("<xmlattr>");

    int schemaVersion = attrs.get<int>("schemaVersion");
    int width = attrs.get<int>("width");
    int height = attrs.get<int>("height");
    std::string bgimage = attrs.get<std::string>("background", {});
    std::string bgcolor = attrs.get<std::string>("bgcolor", {});

    return MainLayout::create(schemaVersion, width, height, bgimage, bgcolor);
}

std::vector<xlf_node>::const_iterator LayoutParser::begin() const
{
    return std::cbegin(m_region_nodes);
}

std::vector<xlf_node>::const_iterator LayoutParser::end() const
{
    return std::cend(m_region_nodes);
}
