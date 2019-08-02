#include "MediaCreatorsRepo.hpp"
#include "MediaResources.hpp"

#include "image/ImageFactory.hpp"
#include "webview/WebViewFactory.hpp"
#include "player/audio/AudioFactory.hpp"
#include "player/video/VideoFactory.hpp"

#include "image/ImageParser.hpp"
#include "webview/WebViewParser.hpp"
#include "player/audio/AudioParser.hpp"
#include "player/video/VideoParser.hpp"

Repository<MediaParser> MediaCreatorsRepo::parsers;
Repository<MediaFactory> MediaCreatorsRepo::factories;

void MediaCreatorsRepo::init()
{
    add({ResourcesXlf::Media::ImageType, ResourcesXlf::Media::NativeRender}, std::make_unique<ImageParser>(), std::make_unique<ImageFactory>());
    add({ResourcesXlf::Media::VideoType, ResourcesXlf::Media::NativeRender}, std::make_unique<VideoParser>(), std::make_unique<VideoFactory>());
    add({ResourcesXlf::Media::HlsType, ResourcesXlf::Media::HtmlRender}, std::make_unique<VideoParser>(), std::make_unique<VideoFactory>());
    add({ResourcesXlf::Media::LocalVideoType, ResourcesXlf::Media::NativeRender}, std::make_unique<VideoParser>(), std::make_unique<VideoFactory>());
    add({ResourcesXlf::Media::AudioType, ResourcesXlf::Media::NativeRender}, std::make_unique<AudioParser>(), std::make_unique<AudioFactory>());
    add({ResourcesXlf::Media::TextType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::EmbeddedType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::TickerType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::WebpageType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::DatasetViewType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
    add({{}, ResourcesXlf::Media::HtmlRender}, std::make_unique<WebViewParser>(), std::make_unique<WebViewFactory>());
}
