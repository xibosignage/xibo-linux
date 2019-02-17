#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/WebView.hpp"
#include "mocks/MockWebViewAdaptor.hpp"



#include "creators/WebViewBuilderTest.hpp"

const bool DEFAULT_TRANSPARENT = false;

class WebViewTest : public BaseTestWithHandler<MockWebViewAdaptor>
{
public:
    auto constructWebView()
    {
        ResourcesXlf::WebViewOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, DEFAULT_TRANSPARENT};

        return WebViewBuilderTest{}.adaptor(unique(&adaptor())).width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT).options(opts).build();
    }

};
