#include "WebViewFactory.hpp"
#include "WebView.hpp"
#include "WebViewResources.hpp"

#include "control/media/Media.hpp"
#include "control/region/RegionResources.hpp"

#include <fstream>
#include <regex>

std::unique_ptr<IMedia> WebViewFactory::createImpl(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    int width = std::stoi(options.at(ResourcesXlf::Region::Width));
    int height = std::stoi(options.at(ResourcesXlf::Region::Height));
    auto transparency = static_cast<WebViewOptions::Transparency>(std::stoi(options.at(ResourcesXlf::WebView::Transparency)));

    return std::make_unique<Media>(baseOptions, createView(baseOptions.uri, width, height, transparency));
}

std::shared_ptr<IWebView> WebViewFactory::createView(const Uri& uri, int width, int height, WebViewOptions::Transparency transparency)
{
    auto webview = std::make_shared<WebView>(width, height);

    webview->load(uri);
    if(transparency == WebViewOptions::Transparency::Enable)
    {
        webview->enableTransparency();
    }

    return webview;
}

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
