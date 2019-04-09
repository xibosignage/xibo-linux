#pragma once

#include "MediaTests.hpp"

#include "mocks/MockWebView.hpp"
#include "creators/WebViewBuilder.hpp"

class WebViewTest : public MediaTest<MockWebViewAdaptor>
{
public:
    auto constructWebView()
    {
        WebViewOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, DEFAULT_TRANSPARENCY, DEFAULT_WEBVIEW_MODE};

        return constructWebView(opts);
    }

    std::unique_ptr<WebView> constructWebView(const WebViewOptions& opts)
    {

        return WebViewBuilder{}.adaptor(unique(&adaptor()))
                               .filesystem(unique(&filesystem()))
                               .width(DEFAULT_WIDTH)
                               .height(DEFAULT_HEIGHT)
                               .options(opts)
                               .build();
    }

};
