#include "RegionFactory.hpp"
#include "control/Region.hpp"

RegionFactory::RegionFactory(const xlf_node& regionNode) :
    m_regionNode(regionNode)
{
}

std::unique_ptr<IRegion> RegionFactory::create()
{
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
