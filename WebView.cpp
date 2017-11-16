#include "WebView.hpp"
#include "Region.hpp"

WebView::WebView(const std::string& filename) :
    m_filename(filename)
{
    m_webView = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    webkit_web_view_load_uri(m_webView, m_filename.c_str());
    webkit_web_view_set_transparent(m_webView, true);

    m_handler.signal_screen_changed().connect(sigc::mem_fun(*this, &WebView::screen_changed));
    screen_changed(m_handler.get_screen());

    auto widget = Glib::wrap(reinterpret_cast<GtkWidget*>(m_webView));
    m_handler.add(*widget);
}

void WebView::init(MyRegion* region, const Point& pos, const Size& size, int zindex)
{
    Media::init(region, pos, size, zindex);

    m_handler.set_size_request(size.width, size.height);
    m_handler.show_all();

    region->add(m_handler);
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

std::string WebView::get_filename() const
{
    return m_filename;
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
