#include "WebViewFactory.hpp"

#include "control/media/VisibleMedia.hpp"
#include "WebView.hpp"

WebViewFactory::WebViewFactory(const WebViewOptions& options) :
    m_options(options)
{
}

std::unique_ptr<Media> WebViewFactory::createModel(const std::shared_ptr<Widget>& view)
{
    return std::make_unique<VisibleMedia>(m_options, *view);
}

ViewInfo WebViewFactory::createView(int width, int height)
{
    auto webview = std::make_shared<WebView>(width, height);
    webview->load(m_options.uri);
    if(m_options.transparency == WebViewOptions::Transparency::Enable)
    {
        webview->enableTransparency();
    }
    return {webview, m_options.geometry.align, m_options.geometry.valign};
}
