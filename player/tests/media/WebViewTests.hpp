#pragma once

#include "MediaTests.hpp"

#include "media/WebView.hpp"
#include "mocks/MockWebViewAdaptor.hpp"
#include "creators/WebViewBuilder.hpp"

const bool DEFAULT_TRANSPARENT = false;

class WebViewTest : public MediaTest<MockWebViewAdaptor>
{
public:
    auto constructWebView()
    {
        return constructWebView(DEFAULT_TRANSPARENT);
    }

    std::unique_ptr<WebView> constructWebView(boost::optional<bool> transparent)
    {
        ResourcesXlf::WebViewOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, transparent};

        return WebViewBuilder{}.adaptor(unique(&adaptor()))
                               .filesystem(unique(&filesystem()))
                               .width(DEFAULT_WIDTH)
                               .height(DEFAULT_HEIGHT)
                               .options(opts)
                               .build();
    }

};
