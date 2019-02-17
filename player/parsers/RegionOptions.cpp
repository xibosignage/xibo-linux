#include "RegionOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::RegionOptions::RegionOptions(int id, int width, int height, int left, int top, int zorder, bool loop) :
    m_id(id), m_width(width), m_height(height), m_left(left), m_top(top), m_zindex(zorder), m_loop(loop)
{
}

ResourcesXlf::RegionOptions::RegionOptions(const xml_node& node)
{
    m_id = node.get<int>(attr(Region::Id));
    m_width = static_cast<int>(node.get<float>(attr(Region::Width)));
    m_height = static_cast<int>(node.get<float>(attr(Region::Height)));
    m_left = static_cast<int>(node.get<float>(attr(Region::Left)));
    m_top = static_cast<int>(node.get<float>(attr(Region::Top)));
    m_zindex = node.get_optional<int>(attr(Region::Zindex));
    m_loop = node.get_optional<bool>(option(Region::Loop));
}

int ResourcesXlf::RegionOptions::id() const
{
    return m_id;
}

int ResourcesXlf::RegionOptions::width() const
{
    return m_width;
}

int ResourcesXlf::RegionOptions::height() const
{
    return m_height;
}

int ResourcesXlf::RegionOptions::left() const
{
    return m_left;
}

int ResourcesXlf::RegionOptions::top() const
{
    return m_top;
}

boost::optional<int> ResourcesXlf::RegionOptions::zorder() const
{
    return m_zindex;
}

boost::optional<bool> ResourcesXlf::RegionOptions::loop() const
{
    return m_loop;
}
