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
    return constructWebView();
}

template<>
auto constructMedia<Image>()
{
    return constructImage();
}

template<>
auto constructMedia<Audio>()
{
}

template<>
auto constructMedia<Video>()
{
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
