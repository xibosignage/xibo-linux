#pragma once

#include "control/media/Media.hpp"
#include "control/media/MediaOptions.hpp"
#include "control/media/webview/WebView.hpp"

#include <memory>

class WebViewFactory
{
public:
    std::unique_ptr<Xibo::Media> create(const MediaOptions& baseOptions, int width, int height, bool transparency);

private:
    std::shared_ptr<Xibo::WebView> createView(const Uri& uri,
                                              int width,
                                              int height,
                                              Xibo::WebView::Transparency transparency);
    void updateViewPortWidth(const Uri& uri, int width);
};
