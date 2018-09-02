#include "RegionParser.hpp"
#include "control/Region.hpp"
#include "parsers/MediaParser.hpp"
#include "utils/utilities.hpp"

RegionParser::RegionParser(const xlf_node& regionNode) :
    m_regionNode(regionNode)
{
    for(auto [regionNodeName, mediaNode]: m_regionNode)
    {
        if(regionNodeName == "media")
        {
            m_mediaNodes.push_back(mediaNode);
        }
    }
}

std::unique_ptr<Region> RegionParser::parse()
{
    //spdlog::get(LOGGER)->trace("Parsing region...");

    auto attrs = m_regionNode.get_child("<xmlattr>");
    auto options = m_regionNode.get_child("options");

    int id = attrs.get<int>("id");
    int width = static_cast<int>(attrs.get<float>("width"));
    int height = static_cast<int>(attrs.get<float>("height"));
    int top = static_cast<int>(attrs.get<float>("top"));
    int left = static_cast<int>(attrs.get<float>("left"));
    int zindex = attrs.get<int>("zindex", 0);
    bool loop = options.get<bool>("loop", false);

    return std::make_unique<Region>(id, width, height, top, left, zindex, loop);
}

std::vector<xlf_node>::const_iterator RegionParser::begin() const
{
    return std::cbegin(m_mediaNodes);
}

std::vector<xlf_node>::const_iterator RegionParser::end() const
{
    return std::cend(m_mediaNodes);
}
