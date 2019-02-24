#include "WebViewOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

WebViewOptions::WebViewOptions(int id, boost::optional<std::string> path, int duration, boost::optional<Transparency> transparency) :
    MediaOptions(id, path, duration), m_transparency(transparency)
{
}

WebViewOptions::WebViewOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_transparency = node.get_optional<Transparency>(ResourcesXlf::option(ResourcesXlf::Media::WebView::Transparent));
}

boost::optional<WebViewOptions::Transparency> WebViewOptions::transparency() const
{
    return m_transparency;
}

std::istream& operator >>(std::istream& in, WebViewOptions::Transparency& transparency)
{
    bool temp;
    in >> temp;
    transparency = static_cast<WebViewOptions::Transparency>(temp);
    return in;
}
