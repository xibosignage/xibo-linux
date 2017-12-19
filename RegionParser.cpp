#include "RegionParser.hpp"

#include <spdlog/spdlog.h>

RegionParser::RegionParser(const boost::property_tree::ptree& tree)
    : m_tree(tree)
{

}

ParsedRegion RegionParser::parse()
{
    spdlog::get(LOGGER)->debug("parse region");

    auto attrs = m_tree.get_child("<xmlattr>");
    auto options = m_tree.get_child("options");

    int id = attrs.get<int>("id");
    int width = static_cast<int>(attrs.get<float>("width"));
    int height = static_cast<int>(attrs.get<float>("height"));
    int top = static_cast<int>(attrs.get<float>("top"));
    int left = static_cast<int>(attrs.get<float>("left"));
    int zindex = attrs.get_optional<int>("zindex").value_or(-1);
    bool loop = options.get_optional<bool>("loop").value_or(false);
    auto type = options.get_optional<std::string>("transitionType").value_or("");
    auto direction = options.get_optional<std::string>("transitionDirection").value_or("");
    int duration = options.get_optional<int>("transitionDuration").value_or(0);

    return std::make_tuple(id, Size{width, height}, Point{left, top}, zindex, loop, Transition{type, direction, duration});
}
