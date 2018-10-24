#pragma once

#include "test_utils.hpp"

#include "media/WebView.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockWebViewAdaptor.hpp"

class WebViewTest : public testing::Test
{
public:
    auto constructWebView()
    {
        auto webview = construct<WebView>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, unique(m_adaptor));
        webview->setDuration(DEFAULT_DURATION);
        return webview;
    }

protected:
    void SetUp() override
    {
        m_adaptor = new testing::NiceMock<MockWebViewAdaptor>;
    }

    void TearDown() override
    {
        m_adaptor = nullptr;
    }

    MockWebViewAdaptor& adaptor()
    {
        return *m_adaptor;
    }

private:
    MockWebViewAdaptor* m_adaptor = nullptr;
};
