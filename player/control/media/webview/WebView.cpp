#include "WebView.hpp"
#include "common/uri/Uri.hpp"

#include <webkit/webkit.h>

namespace ph = std::placeholders;

WebView::WebView(int width, int height) : Widget(m_handler)
{
    m_webView = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    auto widget = Gtk::manage(Glib::wrap(reinterpret_cast<GtkWidget*>(m_webView)));
    m_handler.add(*widget);

    m_sizeAllocateConnection = widget->signal_size_allocate().connect([this](Gtk::Allocation&) {
        reload();
        m_sizeAllocateConnection.disconnect();
    });

    setSize(width, height);
}

void WebView::show()
{
    m_handler.show_all();
}

void WebView::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
    reload();
}

void WebView::reload()
{
    webkit_web_view_reload(m_webView);
}

void WebView::load(const Uri& uri)
{
    webkit_web_view_load_uri(m_webView, uri.string().c_str());
}

void WebView::enableTransparency()
{
    m_handler.signal_screen_changed().connect(std::bind(&WebView::screenChanged, this, ph::_1));
    screenChanged(m_handler.get_screen());

    webkit_web_view_set_transparent(m_webView, true);
}

void WebView::screenChanged(const Glib::RefPtr<Gdk::Screen>& screen)
{
    if (screen)
    {
        auto visual = screen->get_rgba_visual();
        if (visual)
        {
            gtk_widget_set_visual(reinterpret_cast<GtkWidget*>(m_handler.gobj()), visual->gobj());
        }
    }
}

Gtk::ScrolledWindow& WebView::get()
{
    return m_handler;
}
