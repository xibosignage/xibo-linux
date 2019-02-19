#pragma once

#include "MediaTests.hpp"

#include "media/Image.hpp"
#include "mocks/MockImageAdaptor.hpp"
#include "creators/ImageBuilder.hpp"

class ImageTest : public MediaTest<MockImageAdaptor>
{
public:
    auto constructImage()
    {
        return constructImage(DEFAULT_SCALE_TYPE, DEFAULT_ALIGN, DEFAULT_VALIGN);
    }

    std::unique_ptr<Image> constructImage(boost::optional<std::string> scaleType,
                                          boost::optional<std::string> align,
                                          boost::optional<std::string> valign)
    {
        ResourcesXlf::ImageOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, scaleType, align, valign};

        return ImageBuilder{}.adaptor(unique(&adaptor()))
                             .filesystem(unique(&filesystem()))
                             .options(opts)
                             .width(DEFAULT_WIDTH)
                             .height(DEFAULT_HEIGHT)
                             .build();
    }

};

