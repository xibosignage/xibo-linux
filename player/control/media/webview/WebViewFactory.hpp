#pragma once

#include "control/media/MediaFactory.hpp"
#include "WebViewOptions.hpp"

class Widget;

class WebViewFactory : public MediaFactory
{
public:
    WebViewFactory(int width, int height, const WebViewOptions& options);

    std::unique_ptr<Media> create() override;

private:
    std::shared_ptr<Widget> createView(int width, int height);

private:
    WebViewOptions m_options;
    int m_width;
    int m_height;
};
