#include "WebViewFactory.hpp"
#include "WebView.hpp"

#include <fstream>
#include <regex>

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

//    updateFileWidth(m_options.uri, width);
    webview->load(m_options.uri);
    if(m_options.transparency == WebViewOptions::Transparency::Enable)
    {
        webview->enableTransparency();
    }

    return webview;
}

//#include "common/logger/Logging.hpp"
//#include <iostream>

//void WebViewFactory::updateFileWidth(const Uri& uri, int width)
//{
//    std::ifstream in(uri.path());
//    std::ofstream out("temp.txt");
//    std::string line;

//    const std::regex viewPortWidth{"[[ViewPortWidth]]"};

//    while(std::getline(in, line))
//    {
//        out << std::regex_replace(line, viewPortWidth, std::to_string(width)) << std::endl;
//    }
//}
