#include "LayoutOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::LayoutOptions::LayoutOptions(int schemaVersion, int width, int height) :
    m_schemaVersion(schemaVersion), m_width(width), m_height(height)
{
}

ResourcesXlf::LayoutOptions::LayoutOptions(const xml_node& node)
{
    m_schemaVersion = node.get<int>(attr(Layout::SchemaVersion));
    m_width = node.get<int>(attr(Layout::Width));
    m_height = node.get<int>(attr(Layout::Height));
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
