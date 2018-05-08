#include "RegionParser.hpp"
#include "parsers/MediaParser.hpp"

RegionParser::RegionParser(const xlf_node& region_node) :
    m_region_node(region_node)
{

}

ParsedRegion RegionParser::parse_region()
{
    auto region_params = parse_region_params();
    for(auto [region_node_name, media_node]: m_region_node)
    {
        if(region_node_name == "media")
        {
            try
            {
                region_params.media.push_back(MediaParser(media_node).parse_media());
            }
            catch(const boost::property_tree::ptree_bad_path& e)
            {
                spdlog::get(LOGGER)->error("[MediaParser]: Some media won't be rendered - {}", e.what());
            }
        }
    }
    return region_params;
}

ParsedRegion RegionParser::parse_region_params()
{
    spdlog::get(LOGGER)->debug("parse region");

    auto attrs = m_region_node.get_child("<xmlattr>");
    auto options = m_region_node.get_child("options");

    ParsedRegion region_params;
    region_params.id = attrs.get<int>("id");
    region_params.width = static_cast<int>(attrs.get<float>("width"));
    region_params.height = static_cast<int>(attrs.get<float>("height"));
    region_params.top = static_cast<int>(attrs.get<float>("top"));
    region_params.left = static_cast<int>(attrs.get<float>("left"));
    region_params.zindex = attrs.get_optional<int>("zindex");
    region_params.loop = options.get_optional<bool>("loop");

    return region_params;
}
