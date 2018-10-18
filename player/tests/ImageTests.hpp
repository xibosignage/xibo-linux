#pragma once

#include "test_utils.hpp"

#include "media/Image.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockImageAdaptor.hpp"
#include "mocks/MockMedia.hpp"

const Image::ScaleType DEFAULT_SCALE_TYPE = Image::ScaleType::Scaled;
const Image::Align DEFAULT_ALIGN = Image::Align::Center;
const Image::Valign DEFAULT_VALIGN = Image::Valign::Middle;
const int DEFAULT_DURATION = 10;

inline auto constructImage()
{
    auto [image, handler] = construct<Image, MockImageAdaptor>(DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, DEFAULT_VALIGN);
    image->setDuration(DEFAULT_DURATION);
    return std::pair{image, handler};
}

inline testing::NiceMock<MockInvisibleMedia>* createMedia()
{
    auto media = new testing::NiceMock<MockInvisibleMedia>;

    return media;
}
