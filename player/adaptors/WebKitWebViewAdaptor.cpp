#include "WebKitWebViewAdaptor.hpp"
#include "utils/FilePath.hpp"

#include <webkit/webkit.h>

WebKitWebViewAdaptor::WebKitWebViewAdaptor() :
    GtkAdaptor<IWebViewAdaptor>(m_handler)
{
    m_webView = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    auto widget = Gtk::manage(Glib::wrap(reinterpret_cast<GtkWidget*>(m_webView)));
    m_handler.add(*widget);

    m_sizeAllocateConnection = widget->signal_size_allocate().connect([=](Gtk::Allocation&){
       webkit_web_view_reload(m_webView);
       m_sizeAllocateConnection.disconnect();
    });
}

void WebKitWebViewAdaptor::show()
{
    m_handler.show_all();
}

void WebKitWebViewAdaptor::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
    reload();
}

void WebKitWebViewAdaptor::reload()
{
    webkit_web_view_reload(m_webView);
}

void WebKitWebViewAdaptor::load(const FilePath& page)
{
    auto path = "file://" + page.string();
    webkit_web_view_load_uri(m_webView, path.c_str());
}

void WebKitWebViewAdaptor::enableTransparency()
{
    m_handler.signal_screen_changed().connect(sigc::mem_fun(*this, &WebKitWebViewAdaptor::screenChanged));
    screenChanged(m_handler.get_screen());

    webkit_web_view_set_transparent(m_webView, true);
}

void WebKitWebViewAdaptor::screenChanged(const Glib::RefPtr<Gdk::Screen>& screen)
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

Gtk::ScrolledWindow& WebKitWebViewAdaptor::get()
{
    return m_handler;
}
