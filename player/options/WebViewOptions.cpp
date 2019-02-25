#include "WebViewOptions.hpp"
#include "XlfResources.hpp"

#include <boost/property_tree/ptree.hpp>

WebViewOptions::WebViewOptions(int id,
                               boost::optional<std::string> path,
                               int duration,
                               boost::optional<Transparency> transparency,
                               boost::optional<Mode> mode) :
    MediaOptions(id, path, duration), m_transparency(transparency), m_mode(mode)
{
}

WebViewOptions::WebViewOptions(const xml_node& node) :
    MediaOptions(node)
{
    m_transparency = node.get_optional<Transparency>(ResourcesXlf::option(ResourcesXlf::Media::WebView::Transparent));
    m_mode = node.get_optional<Mode>(ResourcesXlf::option(ResourcesXlf::Media::WebView::ModeId));
}

boost::optional<WebViewOptions::Transparency> WebViewOptions::transparency() const
{
    return m_transparency;
}

boost::optional<WebViewOptions::Mode> WebViewOptions::mode() const
{
    return m_mode;
}

std::istream& operator >>(std::istream& in, WebViewOptions::Transparency& transparency)
{
    bool temp;
    in >> temp;
    transparency = static_cast<WebViewOptions::Transparency>(temp);
    return in;
}

std::istream& operator >>(std::istream& in, WebViewOptions::Mode& mode)
{
    const int NATIVE_MODEID = 1;

    int temp;
    in >> temp;
    mode = temp == NATIVE_MODEID ? WebViewOptions::Mode::WebBrowser : WebViewOptions::Mode::FileResource;

    return in;
}
