#include "WebViewOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::WebViewOptions::WebViewOptions(int id, boost::optional<std::string> path, int duration, boost::optional<bool> transparent) :
    MediaOptions(id, path, duration), m_transparent(transparent)
{
}

ResourcesXlf::WebViewOptions::WebViewOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_transparent = node.get_optional<bool>(option(Media::WebView::Transparent));
}

boost::optional<bool> ResourcesXlf::WebViewOptions::transparent() const
{
    return m_transparent;
}
