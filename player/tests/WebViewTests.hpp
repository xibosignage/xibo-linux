#pragma once

#include "test_utils.hpp"

#include "media/WebView.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockWebViewAdaptor.hpp"

inline auto constructWebView()
{
    auto [webview, handler] = construct<WebView, MockWebViewAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH);
    webview->setDuration(DEFAULT_DURATION);
    return std::pair{webview, handler};
}

inline auto constructWebView(std::unique_ptr<MockWebViewAdaptor>&& adaptor)
{
    auto webview = construct<WebView>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::move(adaptor));
    webview->setDuration(DEFAULT_DURATION);
    return webview;
}
