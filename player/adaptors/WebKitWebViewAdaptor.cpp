#include "WebKitWebViewAdaptor.hpp"

#include "adaptors/AdaptorVisitor.hpp"

WebKitWebViewAdaptor::WebKitWebViewAdaptor()
{
    m_webView = reinterpret_cast<WebKitWebView*>(webkit_web_view_new());
    auto widget = Glib::wrap(reinterpret_cast<GtkWidget*>(m_webView));
    m_handler.add(*widget);
}

void WebKitWebViewAdaptor::show()
{
    m_handler.show_all();
}

void WebKitWebViewAdaptor::hide()
{
    m_handler.hide();
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

void WebKitWebViewAdaptor::scale(double scaleX, double scaleY)
{

}

void WebKitWebViewAdaptor::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
}

int WebKitWebViewAdaptor::width() const
{
    int width, _;
    m_handler.get_size_request(width, _);
    return width;
}

int WebKitWebViewAdaptor::height() const
{
    int _, height;
    m_handler.get_size_request(_, height);
    return height;
}

Gtk::Widget* WebKitWebViewAdaptor::get()
{
    return &m_handler;
}

void WebKitWebViewAdaptor::apply(AdaptorVisitor& visitor)
{
    visitor.visit(*this);
}

