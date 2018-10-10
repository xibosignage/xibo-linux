#pragma once

#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockMedia : public IMedia
{
public:
    MockMedia(std::unique_ptr<IWidgetAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(startTimer, void());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_METHOD1(attachAudio, void(std::unique_ptr<IMedia>&& audio));
    MOCK_METHOD1(connect, void(OnMediaTimeout callback));
    MOCK_CONST_METHOD0(duration, int());
    MOCK_METHOD1(setDuration, void(int duration));
    MOCK_METHOD0(handler, IWidgetAdaptor&());
    MOCK_METHOD1(apply, void(MediaVisitor& visitor));

private:
    std::unique_ptr<IWidgetAdaptor> m_handler;

};
