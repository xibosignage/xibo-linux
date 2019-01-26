#include "LayoutOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::LayoutOptions::LayoutOptions(const xml_node& node)
{
    m_schemaVersion = node.get<int>(attr(Layout::SchemaVersion));
    m_width = node.get<int>(attr(Layout::Width));
    m_height = node.get<int>(attr(Layout::Height));
    m_backgroundPath = node.get_optional<std::string>(attr(Layout::BackgroundPath));
    m_backgroundColor = node.get_optional<std::string>(attr(Layout::BackgroundColor));
}

int ResourcesXlf::LayoutOptions::schemaVersion() const
{
    return m_schemaVersion;
}

int ResourcesXlf::LayoutOptions::width() const
{
    return m_width;
}

int ResourcesXlf::LayoutOptions::height() const
{
    return m_height;
}

boost::optional<std::string> ResourcesXlf::LayoutOptions::backgroundPath() const
{
    return m_backgroundPath;
}

boost::optional<std::string> ResourcesXlf::LayoutOptions::backgroundColor() const
{
    return m_backgroundColor;
}
