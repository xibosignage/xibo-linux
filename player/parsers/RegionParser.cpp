#include "RegionParser.hpp"
#include "control/Region.hpp"
#include "parsers/MediaParser.hpp"
#include "utils/utilities.hpp"

RegionParser::RegionParser(const xlf_node& region_node) :
    m_region_node(region_node)
{
    for(auto [region_node_name, media_node]: m_region_node)
    {
        if(region_node_name == "media")
        {
            m_media_nodes.push_back(media_node);
        }
    }
}

std::unique_ptr<Region> RegionParser::parse()
{
    spdlog::get(LOGGER)->trace("Parsing region...");

    auto attrs = m_region_node.get_child("<xmlattr>");
    auto options = m_region_node.get_child("options");

    int id = attrs.get<int>("id");
    int width = static_cast<int>(attrs.get<float>("width"));
    int height = static_cast<int>(attrs.get<float>("height"));
    int top = static_cast<int>(attrs.get<float>("top"));
    int left = static_cast<int>(attrs.get<float>("left"));
    int zindex = attrs.get<int>("zindex", 0);
    bool loop = options.get<bool>("loop", false);

    return Region::create(id, width, height, top, left, zindex, loop);
}

std::vector<xlf_node>::const_iterator RegionParser::begin() const
{
    return std::cbegin(m_media_nodes);
}

std::vector<xlf_node>::const_iterator RegionParser::end() const
{
    return std::cend(m_media_nodes);
}
