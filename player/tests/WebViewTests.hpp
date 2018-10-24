#pragma once

#include "test_utils.hpp"

#include "media/WebView.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockWebViewAdaptor.hpp"

inline auto constructWebView()
{
    auto [webview, handler] = construct<WebView, MockWebViewAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, FilePath{});
    webview->setDuration(DEFAULT_DURATION);
    return std::pair{webview, handler};
}
