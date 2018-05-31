#include "WebView.hpp"
#include "control/Region.hpp"

#include <spdlog/spdlog.h>

WebView::WebView(int id, const Size& size, int duration, const std::string& uri, int modeId, bool transparent) :
    Media(id, duration, (modeId == 1) ? Render::Native : Render::HTML, uri),
    m_transparent(transparent)
{
    auto path = "file://" + uri;
    spdlog::get(LOGGER)->debug(path);

    m_web_view = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    webkit_web_view_load_uri(m_web_view, path.c_str());

    if(m_transparent)
    {
        m_handler.signal_screen_changed().connect(sigc::mem_fun(*this, &WebView::screen_changed));
        screen_changed(m_handler.get_screen());

        webkit_web_view_set_transparent(m_web_view, true);
    }

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

void WebView::stop()
{
    Media::stop();
    m_handler.hide();
}

void WebView::start()
{
    Media::start();
    m_handler.show_all();
    webkit_web_view_reload(m_web_view);
}

void WebView::set_region(Region* region)
{
    Media::set_region(region);

    region->request_handler().connect([=]{
        handler_requested().emit(m_handler, DEFAULT_POINT);
    });
}

bool WebView::transparent() const
{
    return m_transparent;
}
