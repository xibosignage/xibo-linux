#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Image.hpp"
#include "mocks/MockImageAdaptor.hpp"

const MediaGeometry::ScaleType DEFAULT_SCALE_TYPE = MediaGeometry::ScaleType::Scaled;
const MediaGeometry::Align DEFAULT_ALIGN = MediaGeometry::Align::Center;
const MediaGeometry::Valign DEFAULT_VALIGN = MediaGeometry::Valign::Middle;
const MediaGeometry DEFAULT_PROPS{DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, DEFAULT_VALIGN};

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

