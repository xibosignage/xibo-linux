#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/WebView.hpp"
#include "mocks/MockWebViewAdaptor.hpp"
#include "managers/Uri.hpp"

class WebViewTest : public BaseTestWithHandler<MockWebViewAdaptor>
{
public:
    auto constructWebView()
    {
        auto webview = construct<WebView>(DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_URI, unique(&adaptor()));
        webview->setDuration(DEFAULT_DURATION);
        return webview;
    }

};
