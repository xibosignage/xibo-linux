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
    int zindex = attrs.get<int>("zindex");
    bool loop = options.get<bool>("loop", false);
    auto type = options.get<std::string>("transitionType",  std::string{});
    auto direction = options.get<std::string>("transitionDirection", std::string{});
    int duration = options.get<int>("transitionDuration", 0);

    return ParsedRegion{id, Size{width, height}, Point{left, top}, zindex, loop, Transition{type, direction, duration}};
}
