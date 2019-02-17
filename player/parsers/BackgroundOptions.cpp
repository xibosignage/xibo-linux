#include "BackgroundOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::BackgroundOptions::BackgroundOptions(int width, int height, boost::optional<std::string> path, boost::optional<std::string> color) :
    m_width(width), m_height(height), m_path(path), m_color(color)
{
}

ResourcesXlf::BackgroundOptions::BackgroundOptions(const xml_node& node)
{
    m_width = node.get<int>(attr(Background::Width));
    m_height = node.get<int>(attr(Background::Height));
    m_path = node.get_optional<std::string>(attr(Background::Path));
    m_color = node.get_optional<std::string>(attr(Background::Color));
}

int ResourcesXlf::BackgroundOptions::width() const
{
    return m_width;
}

int ResourcesXlf::BackgroundOptions::height() const
{
    return m_height;
}

boost::optional<std::string> ResourcesXlf::BackgroundOptions::path() const
{
    return m_path;
}

boost::optional<std::string> ResourcesXlf::BackgroundOptions::color() const
{
    return m_color;
}
