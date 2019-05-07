#include "WebViewFactory.hpp"

#include "WebView.hpp"

WebViewFactory::WebViewFactory(int width, int height, const WebViewOptions& options) :
    m_options(options),
    m_width(width),
    m_height(height)
{
}

std::unique_ptr<Media> WebViewFactory::create()
{
    return std::make_unique<Media>(m_options, createView(m_width, m_height));
}

std::shared_ptr<Widget> WebViewFactory::createView(int width, int height)
{
    auto webview = std::make_shared<WebView>(width, height);

    webview->load(m_options.uri);
    if(m_options.transparency == WebViewOptions::Transparency::Enable)
    {
        webview->enableTransparency();
    }

    return webview;
}
