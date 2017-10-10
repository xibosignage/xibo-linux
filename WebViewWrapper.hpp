#ifndef WEBVIEWWRAPPER_HPP
#define WEBVIEWWRAPPER_HPP

#include <wx/webview.h>
#include <webkit/webkit.h>

class WebViewWrapper
{
public:
    WebViewWrapper(wxWindow* parent,
                   wxWindowID id,
                   const wxString& url = wxWebViewDefaultURLStr,
                   const wxPoint& pos = wxDefaultPosition,
                   const wxSize& size = wxDefaultSize,
                   const wxString& backend = wxWebViewBackendDefault,
                   long style = 0,
                   const wxString& name = wxWebViewNameStr);

    wxWebView* webView() const;

private:
    wxWebView* m_webView = nullptr;

};

#endif // WEBVIEWWRAPPER_HPP
