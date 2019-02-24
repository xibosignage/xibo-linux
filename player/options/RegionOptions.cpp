#include "RegionOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

RegionOptions::RegionOptions(int id,
                             int width,
                             int height,
                             int left,
                             int top,
                             boost::optional<int> zorder,
                             boost::optional<RegionOptions::Loop> loop) :
    m_id(id), m_width(width), m_height(height), m_left(left), m_top(top), m_zindex(zorder), m_loop(loop)
{
}

RegionOptions::RegionOptions(const xml_node& node)
{
    m_id = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Region::Id));
    m_width = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Width)));
    m_height = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Height)));
    m_left = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Left)));
    m_top = static_cast<int>(node.get<float>(ResourcesXlf::attr(ResourcesXlf::Region::Top)));
    m_zindex = node.get_optional<int>(ResourcesXlf::attr(ResourcesXlf::Region::Zindex));
    m_loop = node.get_optional<RegionOptions::Loop>(ResourcesXlf::option(ResourcesXlf::Region::Loop));
}

int RegionOptions::id() const
{
    return m_id;
}

int RegionOptions::width() const
{
    return m_width;
}

int RegionOptions::height() const
{
    return m_height;
}

int RegionOptions::left() const
{
    return m_left;
}

int RegionOptions::top() const
{
    return m_top;
}

boost::optional<int> RegionOptions::zorder() const
{
    return m_zindex;
}

boost::optional<RegionOptions::Loop> RegionOptions::loop() const
{
    return m_loop;
}

std::istream& operator >>(std::istream& in, RegionOptions::Loop& loop)
{
    bool temp;
    in >> temp;
    loop = static_cast<RegionOptions::Loop>(temp);
    return in;
}
