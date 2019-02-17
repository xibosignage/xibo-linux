#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Video.hpp"
#include "mocks/MockVideoHandler.hpp"


#include "creators/VideoBuilderTest.hpp"

class VideoTest : public BaseTestWithHandler<MockVideoHandler>
{
public:
    auto constructVideo()
    {
        ResourcesXlf::VideoOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, DEFAULT_VIDEO_MUTED, DEFAULT_VIDEO_LOOPED};

        return VideoBuilderTest{}.adaptor(unique(&adaptor())).options(opts).width(DEFAULT_WIDTH).height(DEFAULT_HEIGHT).build();
    }

};
