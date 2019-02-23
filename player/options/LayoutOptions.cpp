#include "LayoutOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

LayoutOptions::LayoutOptions(int width, int height) :
    m_width(width), m_height(height)
{
}

LayoutOptions::LayoutOptions(const xml_node& node)
{
    m_width = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::Width));
    m_height = node.get<int>(ResourcesXlf::attr(ResourcesXlf::Layout::Height));
}

int LayoutOptions::width() const
{
    return m_width;
}

int LayoutOptions::height() const
{
    return m_height;
}
