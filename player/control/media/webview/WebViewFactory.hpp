#pragma once

#include "control/media/MediaFactory.hpp"

#include "WebViewOptions.hpp"

class WebViewFactory : public VisibleMediaFactory
{
public:
    WebViewFactory(const WebViewOptions& options);

    std::unique_ptr<Media> createModel(const std::shared_ptr<Widget>& view) override;
    ViewInfo createView(int width, int height) override;

private:
    WebViewOptions m_options;

};
