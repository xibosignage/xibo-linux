#include "MediaParsersRepo.hpp"

#include "control/media/image/ImageParser.hpp"
#include "control/media/player/audio/AudioNodeParser.hpp"
#include "control/media/player/audio/AudioParser.hpp"
#include "control/media/player/video/VideoParser.hpp"
#include "control/media/webview/WebViewParser.hpp"

#include <gst/gst.h>

Repository<MediaParser> MediaParsersRepo::parsers_;

void MediaParsersRepo::init()
{
    gst_init(nullptr, nullptr);

    add({XlfResources::Media::ImageType, XlfResources::Media::NativeRender}, std::make_unique<ImageParser>());
    add({XlfResources::Media::VideoType, XlfResources::Media::NativeRender}, std::make_unique<VideoParser>());
    add({XlfResources::Media::HlsType, XlfResources::Media::HtmlRender}, std::make_unique<VideoParser>());
    add({XlfResources::Media::LocalVideoType, XlfResources::Media::NativeRender}, std::make_unique<VideoParser>());
    add({XlfResources::Media::AudioType, XlfResources::Media::NativeRender}, std::make_unique<AudioParser>());
    add({XlfResources::Media::AudioNodeType, XlfResources::Media::NativeRender}, std::make_unique<AudioNodeParser>());
    add({XlfResources::Media::TextType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({XlfResources::Media::EmbeddedType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({XlfResources::Media::TickerType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({XlfResources::Media::WebpageType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({XlfResources::Media::DatasetViewType, XlfResources::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({{}, XlfResources::Media::HtmlRender}, std::make_unique<WebViewParser>());
}
