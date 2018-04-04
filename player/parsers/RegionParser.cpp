#include "RegionParser.hpp"
#include "utils/utilities.hpp"
#include "parsers/XlfParser.hpp"

Params RegionParser::parse_region(int region_id)
{
    m_region_id = region_id;

    spdlog::get(LOGGER)->debug("parse region");
    auto region_node = utilities::xlf_parser().region_node(region_id);
    boost::property_tree::ptree region_params;

    static int available_zindex = 0;
    auto attrs = region_node.get_child("<xmlattr>");
    auto options = region_node.get_child("options");
    int id = attrs.get<int>("id");

    region_params.put("id", id);
    region_params.put("width", static_cast<int>(attrs.get<float>("width")));
    region_params.put("height", static_cast<int>(attrs.get<float>("height")));
    region_params.put("top", static_cast<int>(attrs.get<float>("top")));
    region_params.put("left", static_cast<int>(attrs.get<float>("left")));

    auto zindex_optional = attrs.get_optional<int>("zindex");
    int zindex = zindex_optional ? zindex_optional.value() : available_zindex++;
    region_params.put("zindex", zindex);
    region_params.put("loop", options.get_optional<bool>("loop").value_or(false));

    return region_params;
}

std::vector<int> RegionParser::media_ids() const
{
    return utilities::xlf_parser().media_ids(m_region_id);
}
