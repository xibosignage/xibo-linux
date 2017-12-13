#include "WebView.hpp"

WebView::WebView(int id, int duration, const std::string& uri, int modeId, bool transparent) :
    Media(id, duration, (modeId == 1) ? Render::Native : Render::HTML, uri),
    m_transparent(transparent)
{
    m_webView = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    webkit_web_view_load_uri(m_webView, m_uri.c_str());

    m_handler.signal_screen_changed().connect(sigc::mem_fun(*this, &WebView::screen_changed));
    screen_changed(m_handler.get_screen());

    webkit_web_view_set_transparent(m_webView, true);

    // update webkit for set_background
    // check drawing transparent windows over others

    auto widget = Glib::wrap(reinterpret_cast<GtkWidget*>(m_webView));
    m_handler.add(*widget);


// fix it
//    m_handler.set_size_request(size.width, size.height);
//    m_handler.show_all();
}

void WebView::screen_changed(const Glib::RefPtr<Gdk::Screen>& screen)
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

void WebView::hide()
{
    Media::hide();
    m_handler.hide();
}

void WebView::show()
{
    Media::show();
    m_handler.show_all();
}
