#pragma once

#include "test_utils.hpp"

#include "media/Audio.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockAudioHandler.hpp"

inline auto constructAudio()
{
    auto [audio, handler] = construct<Audio, MockAudioHandler>(DEFAULT_PATH);
    audio->setDuration(DEFAULT_DURATION);
    return std::pair{audio, handler};
}

inline auto constructAudio(std::unique_ptr<MockAudioHandler>&& adaptor)
{
    auto audio = construct<Audio>(DEFAULT_PATH, std::move(adaptor));
    audio->setDuration(DEFAULT_DURATION);
    return audio;
}
