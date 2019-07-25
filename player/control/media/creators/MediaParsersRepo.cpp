#include "MediaParsersRepo.hpp"
#include "MediaResources.hpp"

#include "../image/ImageParser.hpp"
#include "../webview/WebViewParser.hpp"
#include "../player/audio/AudioParser.hpp"
#include "../player/video/VideoParser.hpp"

void MediaParsersRepo::init()
{
    add({ResourcesXlf::Media::ImageType, ResourcesXlf::Media::NativeRender}, std::make_unique<ImageParser>());
    add({ResourcesXlf::Media::VideoType, ResourcesXlf::Media::NativeRender}, std::make_unique<VideoParser>());
    add({ResourcesXlf::Media::HlsType, ResourcesXlf::Media::HtmlRender}, std::make_unique<VideoParser>());
    add({ResourcesXlf::Media::LocalVideoType, ResourcesXlf::Media::NativeRender}, std::make_unique<VideoParser>());
    add({ResourcesXlf::Media::AudioType, ResourcesXlf::Media::NativeRender}, std::make_unique<AudioParser>());
    add({ResourcesXlf::Media::TextType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({ResourcesXlf::Media::EmbeddedType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({ResourcesXlf::Media::TickerType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({ResourcesXlf::Media::WebpageType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({ResourcesXlf::Media::DatasetViewType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewParser>());
    add({{}, ResourcesXlf::Media::HtmlRender}, std::make_unique<WebViewParser>());
}
