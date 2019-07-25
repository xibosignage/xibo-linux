#include "MediaFactoriesRepo.hpp"
#include "MediaResources.hpp"

#include "../image/ImageFactory.hpp"
#include "../webview/WebViewFactory.hpp"
#include "../player/audio/AudioFactory.hpp"
#include "../player/video/VideoFactory.hpp"

void MediaFactoriesRepo::init()
{
    add({ResourcesXlf::Media::ImageType, ResourcesXlf::Media::NativeRender}, std::make_unique<ImageFactory>());
    add({ResourcesXlf::Media::VideoType, ResourcesXlf::Media::NativeRender}, std::make_unique<VideoFactory>());
    add({ResourcesXlf::Media::HlsType, ResourcesXlf::Media::HtmlRender}, std::make_unique<VideoFactory>());
    add({ResourcesXlf::Media::LocalVideoType, ResourcesXlf::Media::NativeRender}, std::make_unique<VideoFactory>());
    add({ResourcesXlf::Media::AudioType, ResourcesXlf::Media::NativeRender}, std::make_unique<AudioFactory>());
    add({ResourcesXlf::Media::TextType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::EmbeddedType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::TickerType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::WebpageType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewFactory>());
    add({ResourcesXlf::Media::DatasetViewType, ResourcesXlf::Media::NativeRender}, std::make_unique<WebViewFactory>());
    add({{}, ResourcesXlf::Media::HtmlRender}, std::make_unique<WebViewFactory>());

}
