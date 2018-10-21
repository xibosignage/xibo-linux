#pragma once

#include "test_utils.hpp"

#include "media/WebView.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockWebViewAdaptor.hpp"
#include "mocks/MockMedia.hpp"

const int DEFAULT_DURATION = 10;

inline auto constructWebView()
{
    auto [webview, handler] = construct<WebView, MockWebViewAdaptor>();
    webview->setDuration(DEFAULT_DURATION);
    return std::pair{webview, handler};
}

inline testing::NiceMock<MockInvisibleMedia>* createMedia()
{
    auto media = new testing::NiceMock<MockInvisibleMedia>;

    return media;
}
