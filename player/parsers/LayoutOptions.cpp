#include "LayoutOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

LayoutOptions::LayoutOptions(int schemaVersion, int width, int height) :
    m_schemaVersion(schemaVersion), m_width(width), m_height(height)
{
}

LayoutOptions::LayoutOptions(const xml_node& node)
{
    m_schemaVersion = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::SchemaVersion));
    m_width = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::Width));
    m_height = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::Height));
}

int LayoutOptions::schemaVersion() const
{
    return m_schemaVersion;
}

int LayoutOptions::width() const
{
    return m_width;
}

int LayoutOptions::height() const
{
    return m_height;
}
