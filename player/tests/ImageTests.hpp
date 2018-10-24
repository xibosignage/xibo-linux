#pragma once

#include "test_utils.hpp"

#include "media/Image.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockImageAdaptor.hpp"

const ImageProperties::ScaleType DEFAULT_SCALE_TYPE = ImageProperties::ScaleType::Scaled;
const ImageProperties::Align DEFAULT_ALIGN = ImageProperties::Align::Center;
const ImageProperties::Valign DEFAULT_VALIGN = ImageProperties::Valign::Middle;
const ImageProperties DEFAULT_PROPS{DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, DEFAULT_VALIGN};

inline auto constructImage()
{
    auto [image, handler] = construct<Image, MockImageAdaptor>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, DEFAULT_PROPS);
    image->setDuration(DEFAULT_DURATION);
    return std::pair{image, handler};
}

inline auto constructImage(std::unique_ptr<MockImageAdaptor>&& adaptor)
{
    auto image = construct<Image>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, DEFAULT_PROPS, std::move(adaptor));
    image->setDuration(DEFAULT_DURATION);
    return image;
}
