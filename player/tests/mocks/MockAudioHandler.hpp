#pragma once

#include "media/IAudioHandler.hpp"
#include <gmock/gmock.h>

class MockAudioHandler : public IAudioHandler
{
public:
    MOCK_METHOD1(load, void(const FilePath& path));
    MOCK_METHOD0(play, void());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(stopPlayback, void());
    MOCK_METHOD1(connect, void(OnAudioFinished callback));
    MOCK_METHOD1(setVolume, void(int volume));

};
