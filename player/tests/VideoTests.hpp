#pragma once

#include "test_utils.hpp"

#include "media/Video.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockVideoHandler.hpp"

inline auto constructVideo()
{
    auto [video, handler] = construct<Video, MockVideoHandler>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH);
    video->setDuration(DEFAULT_DURATION);
    return std::pair{video, handler};
}

inline auto constructVideo(std::unique_ptr<MockVideoHandler>&& adaptor)
{
    auto video = construct<Video>(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_PATH, std::move(adaptor));
    video->setDuration(DEFAULT_DURATION);
    return video;
}
