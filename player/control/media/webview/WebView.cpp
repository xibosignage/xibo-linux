#include "WebView.hpp"
#include "common/uri/Uri.hpp"

#include <webkit/webkit.h>

namespace ph = std::placeholders;

WebView::WebView(int width, int height) : Widget(handler_)
{
    webView_ = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    auto widget = Gtk::manage(Glib::wrap(reinterpret_cast<GtkWidget*>(webView_)));
    handler_.add(*widget);

    sizeAllocateConnection_ = widget->signal_size_allocate().connect([this](Gtk::Allocation&) {
        reload();
        sizeAllocateConnection_.disconnect();
    });

    setSize(width, height);
}

void WebView::show()
{
    handler_.show_all();
}

void WebView::setSize(int width, int height)
{
    handler_.set_size_request(width, height);
    reload();
}

void WebView::reload()
{
    webkit_web_view_reload(webView_);
}

void WebView::load(const Uri& uri)
{
    webkit_web_view_load_uri(webView_, uri.string().c_str());
}

void WebView::enableTransparency()
{
    handler_.signal_screen_changed().connect(std::bind(&WebView::screenChanged, this, ph::_1));
    screenChanged(handler_.get_screen());

    webkit_web_view_set_transparent(webView_, true);
}

void WebView::screenChanged(const Glib::RefPtr<Gdk::Screen>& screen)
{
    if (screen)
    {
        auto visual = screen->get_rgba_visual();
        if (visual)
        {
            gtk_widget_set_visual(reinterpret_cast<GtkWidget*>(handler_.gobj()), visual->gobj());
        }
    }
}

Gtk::ScrolledWindow& WebView::get()
{
    return handler_;
}
