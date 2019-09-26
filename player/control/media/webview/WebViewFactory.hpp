#pragma once

#include "WebViewOptions.hpp"
#include "control/media/MediaFactory.hpp"

class IWebView;

class WebViewFactory : public MediaFactory
{
protected:
    std::unique_ptr<IMedia> create(const MediaOptions& baseOptions, const ExtraOptions& options) override;

private:
    std::shared_ptr<IWebView> createView(const Uri& uri, int width, int height,
                                         WebViewOptions::Transparency transparency);
    void updateViewPortWidth(const Uri& uri, int width);
};
