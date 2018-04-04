#include "LayoutParser.hpp"
#include "utils/utilities.hpp"
#include "parsers/XlfParser.hpp"

Params LayoutParser::parse_layout()
{
    auto layout_node = utilities::xlf_parser().layout_node();
    spdlog::get(LOGGER)->debug("parse layout");
    boost::property_tree::ptree layout_params;

    auto attrs = layout_node.get_child("<xmlattr>");
    layout_params.put("schemaVersion", attrs.get<int>("schemaVersion"));
    layout_params.put("width", attrs.get<int>("width"));
    layout_params.put("height", attrs.get<int>("height"));
    layout_params.put("background", attrs.get_optional<std::string>("background").value_or(std::string{}));
    layout_params.put("bgcolor", attrs.get_optional<std::string>("bgcolor").value_or(std::string{}));

    return layout_params;
}

std::vector<int> LayoutParser::regions_ids() const
{
    return utilities::xlf_parser().regions_ids();
}
