#include "WebViewGtk.hpp"

#include "common/types/Uri.hpp"

#include <webkit/webkit.h>

namespace ph = std::placeholders;

WebViewGtk::WebViewGtk(int width, int height) : WidgetGtk{handler_}
{
    WidgetGtk::setSize(width, height);

    webView_ = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    auto widget = Gtk::manage(Glib::wrap(reinterpret_cast<GtkWidget*>(webView_)));
    handler_.add(*widget);

    // GTK+ doesn't change the physical size of the widget immediately. Instead, we need to wait for size-allocate
    // signal. However, it emits too often even if the widget hasn't been actually resized so we block it after handler
    // execution and unblock during next setSize
    // We need this because WebView doesn't reload its content even when the parent widget has been resized
    sizeAllocateConnection_ = handler_.signal_size_allocate().connect([this](Gtk::Allocation&) {
        reload();
        sizeAllocateConnection_.block();
    });
}

void WebViewGtk::show()
{
    handler_.show_all();
}

void WebViewGtk::setSize(int width, int height)
{
    sizeAllocateConnection_.unblock();
    WidgetGtk::setSize(width, height);
}

void WebViewGtk::reload()
{
    webkit_web_view_reload(webView_);
}

void WebViewGtk::load(const Uri& uri)
{
    webkit_web_view_load_uri(webView_, uri.string().c_str());
}

void WebViewGtk::enableTransparency()
{
    handler_.signal_screen_changed().connect(std::bind(&WebViewGtk::screenChanged, this, ph::_1));
    screenChanged(handler_.get_screen());

    webkit_web_view_set_transparent(webView_, true);
}

void WebViewGtk::screenChanged(const Glib::RefPtr<Gdk::Screen>& screen)
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

Gtk::ScrolledWindow& WebViewGtk::handler()
{
    return handler_;
}
