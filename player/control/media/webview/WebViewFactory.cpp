#include "WebViewFactory.hpp"
#include "WebView.hpp"
#include "WebViewResources.hpp"

#include "control/media/Media.hpp"
#include "control/media/MediaResources.hpp"

#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/fs/Resources.hpp"

#include <fstream>
#include <regex>

const std::regex ViewPortWidth{"(content=\"width=)(.*)(\".*)"};

std::unique_ptr<IMedia> WebViewFactory::create(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    int width = std::stoi(options.at(XlfResources::Media::Width));
    int height = std::stoi(options.at(XlfResources::Media::Height));
    auto transparency =
        static_cast<WebViewOptions::Transparency>(std::stoi(options.at(XlfResources::WebView::Transparency)));

    updateViewPortWidth(baseOptions.uri, width);

    return std::make_unique<Media>(baseOptions, createView(baseOptions.uri, width, height, transparency));
}

std::shared_ptr<IWebView> WebViewFactory::createView(const Uri& uri, int width, int height,
                                                     WebViewOptions::Transparency transparency)
{
    auto webview = std::make_shared<WebView>(width, height);

    webview->load(uri);
    if (transparency == WebViewOptions::Transparency::Enable)
    {
        webview->enableTransparency();
    }

    return webview;
}

void WebViewFactory::updateViewPortWidth(const Uri& uri, int width)
{
    auto filename = uri.path().substr(1);
    auto path = Resources::directory() / filename;
    std::string fileContent;
    {
        std::ifstream stream(path.string());
        std::stringstream buffer;
        buffer << stream.rdbuf();

        std::regex_search(fileContent, ViewPortWidth);
        fileContent = std::regex_replace(buffer.str(), ViewPortWidth, "$1 " + std::to_string(width) + "$3");
    }
    {
        std::ofstream stream(path.string(), std::ios::out | std::ios::trunc);
        stream << fileContent;
    }
}
