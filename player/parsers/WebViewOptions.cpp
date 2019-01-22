#include "WebViewOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::WebViewOptions::WebViewOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_transparent = node.get_optional<bool>(option(Media::WebView::Transparent));
}

boost::optional<bool> ResourcesXlf::WebViewOptions::transparent() const
{
    return m_transparent;
}
