#pragma once

#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockMedia : public IMedia
{
public:
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(start, void());
    MOCK_CONST_METHOD0(isRunning, bool());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_METHOD0(startTimer, void());
    MOCK_METHOD1(attachAudio, void(std::unique_ptr<IMedia> audio));
    MOCK_METHOD1(connect, void(OnMediaTimeout callback));
    MOCK_METHOD0(handler, IWidgetAdaptor&());
    MOCK_METHOD1(apply, void(MediaVisitor& visitor));
    MOCK_CONST_METHOD0(id, int());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_CONST_METHOD0(duration, int());
    MOCK_CONST_METHOD0(render, Render());
    MOCK_CONST_METHOD0(uri, const std::string&());
};
