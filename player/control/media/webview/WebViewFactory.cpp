#include "WebViewFactory.hpp"

#include "control/media/webview/WebView.hpp"
#include "control/media/webview/WebViewWidgetFactory.hpp"

#include "control/media/MediaImpl.hpp"
#include "control/media/MediaResources.hpp"

#include "common/fs/FilePath.hpp"
#include "common/fs/FileSystem.hpp"
#include "common/fs/Resources.hpp"

#include <fstream>
#include <regex>

const std::regex ViewPortWidth{"(content=\"width=)(.*)(\".*)"};

std::unique_ptr<Xibo::Media> WebViewFactory::create(const MediaOptions& options,
                                                    int width,
                                                    int height,
                                                    bool transparency)
{
    updateViewPortWidth(options.uri, width);

    auto media = std::make_unique<MediaImpl>(options);
    media->setWidget(createView(options.uri, width, height, static_cast<Xibo::WebView::Transparency>(transparency)));
    return media;
}

std::shared_ptr<Xibo::WebView> WebViewFactory::createView(const Uri& uri,
                                                          int width,
                                                          int height,
                                                          Xibo::WebView::Transparency transparency)
{
    auto webview = WebViewWidgetFactory::create(width, height);

    webview->load(uri);
    if (transparency == Xibo::WebView::Transparency::Enable)
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
