#pragma once

#include "media/IMedia.hpp"

#include <gmock/gmock.h>

template<typename Interface>
class MockMedia : public Interface
{
public:
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(startTimer, void());
    MOCK_METHOD1(attachAudio, void(std::unique_ptr<IMedia>&& audio));
    MOCK_METHOD1(connect, void(OnMediaTimeout callback));
    MOCK_CONST_METHOD0(duration, int());
    MOCK_METHOD1(setDuration, void(int duration));
    MOCK_METHOD1(apply, void(MediaVisitor& visitor));

};

class MockVisibleMedia : public MockMedia<IVisibleMedia>
{
public:
    MockVisibleMedia(std::unique_ptr<IWidgetAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(handler, IWidgetAdaptor&());

private:
    std::unique_ptr<IWidgetAdaptor> m_handler;

};

class MockInvisibleMedia : public MockMedia<IInvisibleMedia>
{
};
