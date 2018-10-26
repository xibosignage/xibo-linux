#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Audio.hpp"
#include "media/MediaVisitor.hpp"

#include "mocks/MockAudioHandler.hpp"

class AudioTest : public BaseTestWithHandler<MockAudioHandler>
{
public:
    auto constructAudio()
    {
        auto audio = construct<Audio>(DEFAULT_PATH, unique(&adaptor()));
        audio->setDuration(DEFAULT_DURATION);
        return audio;
    }

};

