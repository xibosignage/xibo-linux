#include "WebViewOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

ResourcesXlf::WebViewOptions::WebViewOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_transparent = node.get_optional<bool>(option(Media::WebView::Transparent));
    m_modeId = node.get_optional<int>(option(Media::WebView::ModeId));
}

boost::optional<bool> ResourcesXlf::WebViewOptions::transparent() const
{
    return m_transparent;
}

boost::optional<int> ResourcesXlf::WebViewOptions::modeId() const
{
    return m_modeId;
}
