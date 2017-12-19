#include "WebView.hpp"

#include <boost/filesystem.hpp>

WebView::WebView(const Size& size, uint id, uint duration, bool use_duration, const std::string& uri, int modeId, bool transparent) :
    Media(id, duration, use_duration, (modeId == 1) ? Render::Native : Render::HTML, uri),
    m_transparent(transparent)
{
    auto path = "file://" + boost::filesystem::current_path().string() + "/TwitterMetro/" + m_uri;

    m_web_view = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    webkit_web_view_load_uri(m_web_view, path.c_str());

    if(m_transparent)
    {
        m_handler.signal_screen_changed().connect(sigc::mem_fun(*this, &WebView::screen_changed));
        screen_changed(m_handler.get_screen());

        webkit_web_view_set_transparent(m_web_view, true);
    }

    // update webkit for set_background
    // check drawing transparent windows over others

    auto widget = Glib::wrap(reinterpret_cast<GtkWidget*>(m_web_view));
    m_handler.add(*widget);

    m_handler.set_size_request(size.width, size.height);
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

Gtk::Widget& WebView::handler()
{
    return m_handler;
}
