#include "RegionParser.hpp"

#include "RegionResources.hpp"

const int DEFAULT_REGION_ZINDEX = 0;
const bool DEFAULT_REGION_LOOP = false;

RegionParser::RegionParser(const xml_node& regionNode) :
    m_regionNode(regionNode)
{
}

RegionOptions RegionParser::parse()
{
    int id = m_regionNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Region::Id));
    int width = static_cast<int>(m_regionNode.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Width)));
    int height = static_cast<int>(m_regionNode.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Height)));
    int left = static_cast<int>(m_regionNode.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Left)));
    int top = static_cast<int>(m_regionNode.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Top)));
    int zindex = m_regionNode.get<int>(ResourcesXlf::attr(ResourcesXlf::Region::Zindex), DEFAULT_REGION_ZINDEX);
    auto loop = m_regionNode.get<bool>(ResourcesXlf::option(ResourcesXlf::Region::Loop), DEFAULT_REGION_LOOP);

    return RegionOptions{id, width, height, left, top, zindex, static_cast<RegionOptions::Loop>(loop)};
}
