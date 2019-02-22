#include "BackgroundOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

BackgroundOptions::BackgroundOptions(int width, int height, boost::optional<std::string> path, boost::optional<std::string> color) :
    m_width(width), m_height(height), m_path(path), m_color(color)
{
}

BackgroundOptions::BackgroundOptions(const xml_node& node)
{
    m_width = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Background::Width));
    m_height = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Background::Height));
    m_path = node.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::Background::Path));
    m_color = node.get_optional<std::string>(ResourcesXlf::attr(ResourcesXlf::Background::Color));
}

int BackgroundOptions::width() const
{
    return m_width;
}

int BackgroundOptions::height() const
{
    return m_height;
}

boost::optional<std::string> BackgroundOptions::path() const
{
    return m_path;
}

boost::optional<std::string> BackgroundOptions::color() const
{
    return m_color;
}
