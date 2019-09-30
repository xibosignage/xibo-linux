#pragma once

#ifdef USE_GTK
#include "control/media/webview/WebViewGtk.hpp"
#endif
#include "control/media/webview/WebView.hpp"

namespace WebViewWidgetFactory
{
    inline std::unique_ptr<Xibo::WebView> create(int width, int height)
    {
#ifdef USE_GTK
        return std::make_unique<WebViewGtk>(width, height);
#else
        return nullptr;
#endif
    }
}
