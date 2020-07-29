#include "WebViewFactory.hpp"

#include "control/media/webview/WebView.hpp"
#include "control/media/webview/WebViewWidgetFactory.hpp"

#include "control/media/MediaImpl.hpp"
#include "control/media/MediaResources.hpp"

#include "common/fs/FileSystem.hpp"
#include "common/fs/Resource.hpp"

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
    Resource path{filename};

    auto originalContent = FileSystem::readFromFile(path);
    auto updatedContent = std::regex_replace(originalContent, ViewPortWidth, "$1 " + std::to_string(width) + "$3");

    FileSystem::writeToFile(path, updatedContent);
}
