#include "WebViewWrapper.hpp"

static void UpdateVisualRgba(GtkWidget* widget, GdkScreen* WXUNUSED(old_screen), gpointer WXUNUSED(userdata))
{
    /* To check if the display supports alpha channels, get the visual */
    GdkScreen *screen = gtk_widget_get_screen(widget);
    GdkVisual *visual = gdk_screen_get_rgba_visual(screen);

    if(!visual)
    {
        visual = gdk_screen_get_system_visual(screen);
    }

    gtk_widget_set_visual(widget, visual);
}

WebViewWrapper::WebViewWrapper(wxWindow* parent, wxWindowID id, const wxString& url,  const wxPoint& pos,
                               const wxSize& size, const wxString& backend, long style, const wxString& name)
{
    auto parentGtkWindow = parent->GetHandle();

    g_signal_connect(parentGtkWindow, "screen-changed", G_CALLBACK(UpdateVisualRgba), nullptr);
    UpdateVisualRgba(parentGtkWindow, nullptr, nullptr);

    m_webView = wxWebView::New(parent, id, url, pos, size, backend, style, name);


    WebKitWebView* nativeWebView = WEBKIT_WEB_VIEW(m_webView->GetNativeBackend());
    webkit_web_view_set_transparent(nativeWebView, true);
}

wxWebView* WebViewWrapper::webView() const
{
    return m_webView;
}
