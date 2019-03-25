#pragma once

#include "MediaTests.hpp"

#include "media/Video.hpp"
#include "mocks/MockVideoHandler.hpp"
#include "creators/VideoBuilder.hpp"

class VideoTest : public MediaTest<MockVideoHandler>
{
public:
    auto constructVideo()
    {
        return constructVideo(DEFAULT_VIDEO_MUTED, DEFAULT_VIDEO_LOOPED);
    }

    std::unique_ptr<Video> constructVideo(boost::optional<VideoOptions::Mute> muted, boost::optional<VideoOptions::Loop> looped)
    {
        VideoOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, muted, looped};

        return VideoBuilder{}.adaptor(unique(&adaptor()))
                             .filesystem(unique(&filesystem()))
                             .options(opts)
                             .width(DEFAULT_WIDTH)
                             .height(DEFAULT_HEIGHT)
                             .build();
    }

};
