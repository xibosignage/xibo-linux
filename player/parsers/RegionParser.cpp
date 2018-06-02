#include "RegionParser.hpp"
#include "control/Region.hpp"
#include "utils/utilities.hpp"
#include "parsers/MediaParser.hpp"

RegionParser::RegionParser(const xlf_node& region_node) :
    m_region_node(region_node)
{

}

std::unique_ptr<Region> RegionParser::parse()
{
    auto p = parse_params();
    auto region = Region::create(p.id, p.width, p.height, p.top, p.left, p.zindex, p.loop);

    for(auto [region_node_name, media_node]: m_region_node)
    {
        if(region_node_name == "media")
        {
            try
            {
                region->add_media(utilities::get_media_parser(m_region_node, media_node)->parse());
            }
            catch(const boost::property_tree::ptree_bad_path& e)
            {
                spdlog::get(LOGGER)->error("[MediaParser]: Some media won't be rendered - {}", e.what());
            }
        }
    }
    return region;
}

RegionParser::ParsedRegion RegionParser::parse_params()
{
    spdlog::get(LOGGER)->debug("parse region");

    auto attrs = m_region_node.get_child("<xmlattr>");
    auto options = m_region_node.get_child("options");

    ParsedRegion params;
    params.id = attrs.get<int>("id");
    params.width = static_cast<int>(attrs.get<float>("width"));
    params.height = static_cast<int>(attrs.get<float>("height"));
    params.top = static_cast<int>(attrs.get<float>("top"));
    params.left = static_cast<int>(attrs.get<float>("left"));
    params.zindex = attrs.get<int>("zindex", 0);
    params.loop = options.get<bool>("loop", false);
    return params;
}
