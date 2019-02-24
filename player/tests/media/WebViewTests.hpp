#pragma once

#include "MediaTests.hpp"

#include "media/WebView.hpp"
#include "mocks/MockWebViewAdaptor.hpp"
#include "creators/WebViewBuilder.hpp"

class WebViewTest : public MediaTest<MockWebViewAdaptor>
{
public:
    auto constructWebView()
    {
        return constructWebView(DEFAULT_TRANSPARENCY);
    }

    std::unique_ptr<WebView> constructWebView(boost::optional<WebViewOptions::Transparency> transparency)
    {
        WebViewOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, transparency};

        return WebViewBuilder{}.adaptor(unique(&adaptor()))
                               .filesystem(unique(&filesystem()))
                               .width(DEFAULT_WIDTH)
                               .height(DEFAULT_HEIGHT)
                               .options(opts)
                               .build();
    }

};
