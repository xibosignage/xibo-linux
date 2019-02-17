#pragma once

#include "BaseTestWithHandler.hpp"

#include "media/Audio.hpp"
#include "mocks/MockAudioHandler.hpp"

const int DEFAULT_VOLUME = 100;

#include "creators/AudioBuilderTest.hpp"

class AudioTest : public BaseTestWithHandler<MockAudioHandler>
{
public:
    auto constructAudio()
    {
        ResourcesXlf::AudioOptions opts{DEFAULT_ID, DEFAULT_PATH.string(), DEFAULT_DURATION, DEFAULT_AUDIO_MUTED, DEFAULT_AUDIO_LOOPED, DEFAULT_VOLUME};

        return AudioBuilderTest{}.adaptor(unique(&adaptor())).options(opts).build();
    }

};

