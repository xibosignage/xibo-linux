#pragma once

#include "media/IVideoHandler.hpp"
#include <gmock/gmock.h>

class MockVideoHandler : public IVideoHandler
{
public:
    MOCK_METHOD1(load, void(const FilePath& path));
    MOCK_METHOD0(play, void());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(stopPlayback, void());
    MOCK_METHOD1(connect, void(OnVideoFinished callback));
    MOCK_METHOD1(setVolume, void(int volume));

    MOCK_METHOD0(videoWindow, IWidgetAdaptor&());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());

};
