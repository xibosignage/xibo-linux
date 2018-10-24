#pragma once

#include "test_utils.hpp"

#include "WebViewTests.hpp"
#include "ImageTests.hpp"
#include "AudioTests.hpp"
#include "VideoTests.hpp"

#include "media/MediaVisitor.hpp"
#include "mocks/MockMedia.hpp"

template<typename T>
auto constructMedia();

template<>
auto constructMedia<WebView>()
{
    auto webview = construct<WebView>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::make_unique<testing::NiceMock<MockWebViewAdaptor>>());
    webview->setDuration(DEFAULT_DURATION);
    return webview;
}

template<>
auto constructMedia<Image>()
{
    auto image = construct<Image>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, DEFAULT_PROPS, std::make_unique<testing::NiceMock<MockImageAdaptor>>());
    image->setDuration(DEFAULT_DURATION);
    return image;
}

template<>
auto constructMedia<Audio>()
{
    auto audio = construct<Audio>(DEFAULT_PATH, std::make_unique<testing::NiceMock<MockAudioHandler>>());
    audio->setDuration(DEFAULT_DURATION);
    return audio;
}

template<>
auto constructMedia<Video>()
{
    auto video = construct<Video>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::make_unique<testing::NiceMock<MockVideoHandler>>());
    video->setDuration(DEFAULT_DURATION);
    return video;
}

inline testing::NiceMock<MockInvisibleMedia>* createMedia()
{
    auto media = new testing::NiceMock<MockInvisibleMedia>;

    return media;
}


template<typename T>
class MediaTest : public testing::Test
{

};
