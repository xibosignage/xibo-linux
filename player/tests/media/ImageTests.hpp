#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Image.hpp"
#include "mocks/MockImageAdaptor.hpp"




#include "creators/ImageBuilderTests.hpp"

class ImageTest : public BaseTestWithHandler<MockImageAdaptor>
{
public:
    auto constructImage()
    {
        ResourcesXlf::ImageOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, DEFAULT_VALIGN};

        return ImageBuilderTest{}.adaptor(unique(&adaptor())).options(opts).width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT).build();
    }

};

