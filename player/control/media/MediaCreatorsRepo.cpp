#include "MediaCreatorsRepo.hpp"
#include "MediaResources.hpp"

#include "image/ImageFactory.hpp"
#include "player/audio/AudioFactory.hpp"
#include "player/video/VideoFactory.hpp"
#include "webview/WebViewFactory.hpp"

#include "image/ImageParser.hpp"
#include "player/audio/AudioNodeParser.hpp"
#include "player/audio/AudioParser.hpp"
#include "player/video/VideoParser.hpp"
#include "webview/WebViewParser.hpp"

Repository<MediaParser> MediaCreatorsRepo::parsers;
Repository<MediaFactory> MediaCreatorsRepo::factories;

void MediaCreatorsRepo::init()
{
    add({XlfResources::Media::ImageType, XlfResources::Media::NativeRender}, std::make_unique<ImageParser>(),
        std::make_unique<ImageFactory>());
    add({XlfResources::Media::VideoType, XlfResources::Media::NativeRender}, std::make_unique<VideoParser>(),
        std::make_unique<VideoFactory>());
    add({XlfResources::Media::HlsType, XlfResources::Media::HtmlRender}, std::make_unique<VideoParser>(),
        std::make_unique<VideoFactory>());
    add({XlfResources::Media::LocalVideoType, XlfResources::Media::NativeRender}, std::make_unique<VideoParser>(),
        std::make_unique<VideoFactory>());
    add({XlfResources::Media::AudioType, XlfResources::Media::NativeRender}, std::make_unique<AudioParser>(),
        std::make_unique<AudioFactory>());
    add({XlfResources::Media::AudioNodeType, XlfResources::Media::NativeRender}, std::make_unique<AudioNodeParser>(),
        std::make_unique<AudioFactory>());
    add({XlfResources::Media::TextType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>(),
        std::make_unique<WebViewFactory>());
    add({XlfResources::Media::EmbeddedType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>(),
        std::make_unique<WebViewFactory>());
    add({XlfResources::Media::TickerType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>(),
        std::make_unique<WebViewFactory>());
    add({XlfResources::Media::WebpageType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>(),
        std::make_unique<WebViewFactory>());
    add({XlfResources::Media::DatasetViewType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>(),
        std::make_unique<WebViewFactory>());
    add({{}, XlfResources::Media::HtmlRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
}
