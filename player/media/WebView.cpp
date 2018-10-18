#include "WebView.hpp"

#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <filesystem>

WebView::WebView(int width, int height, int duration, const std::string& uri, bool transparent) :
    m_transparent(transparent)
{
    if(std::filesystem::exists(uri))
    {
        auto path = "file://" + uri;
        spdlog::get(LOGGER)->trace("WebView file {}", path);

        m_webView = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
        webkit_web_view_load_uri(m_webView, path.c_str());

        if(m_transparent)
        {
            m_handler.signal_screen_changed().connect(sigc::mem_fun(*this, &WebView::screenChanged));
            screenChanged(m_handler.get_screen());

            webkit_web_view_set_transparent(m_webView, true);
        }

        auto widget = Glib::wrap(reinterpret_cast<GtkWidget*>(m_webView));
        m_handler.add(*widget);
        m_handler.set_size_request(width, height);
    }
    else
    {
        spdlog::get(LOGGER)->error("Could not find webview: {}", uri);
    }
}

void WebView::screenChanged(const Glib::RefPtr<Gdk::Screen>& screen)
{
    if(screen)
    {
        auto visual = screen->get_rgba_visual();
        if(visual)
        {
            gtk_widget_set_visual(reinterpret_cast<GtkWidget*>(m_handler.gobj()), visual->gobj());
        }
    }
}

void WebView::doStop()
{
    m_handler.hide();
}

void WebView::doStart()
{
    m_handler.show_all();
    webkit_web_view_reload(m_webView);
}

//void WebView::setSize(int width, int height)
//{
//    m_handler.set_size_request(width, height);
//}

#include "media/MediaVisitor.hpp"
void WebView::apply(MediaVisitor& visitor)
{
    visitor.visit(*this);
}

bool WebView::transparent() const
{
    return m_transparent;
}

#include "adaptors/GtkImageAdaptor.hpp"
IWidgetAdaptor& WebView::handler()
{
    return *new GtkImageAdaptor;
}
