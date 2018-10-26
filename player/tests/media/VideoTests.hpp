#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Video.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockVideoHandler.hpp"

class VideoTest : public BaseTestWithHandler<MockVideoHandler>
{
public:
    auto constructVideo()
    {
        auto video = construct<Video>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, unique(&adaptor()));
        video->setDuration(DEFAULT_DURATION);
        return video;
    }

};
