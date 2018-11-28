#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Image.hpp"
#include "media/MediaVisitor.hpp"
#include "mocks/MockImageAdaptor.hpp"

const ImageProperties::ScaleType DEFAULT_SCALE_TYPE = ImageProperties::ScaleType::Scaled;
const ImageProperties::Align DEFAULT_ALIGN = ImageProperties::Align::Center;
const ImageProperties::Valign DEFAULT_VALIGN = ImageProperties::Valign::Middle;
const ImageProperties DEFAULT_PROPS{DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, DEFAULT_VALIGN};

class ImageTest : public BaseTestWithHandler<MockImageAdaptor>
{
public:
    auto constructImage()
    {
        auto image = construct<Image>(DEFAULT_ID, DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, DEFAULT_PROPS, unique(&adaptor()));
        image->setDuration(DEFAULT_DURATION);
        return image;
    }

};

