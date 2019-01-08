#pragma once

#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockMedia : public IMedia
{
public:
    MOCK_CONST_METHOD0(duration, int());
    MOCK_METHOD1(setDuration, void(int duration));
    MOCK_CONST_METHOD0(id, int());
    MOCK_METHOD1(apply, void(MediaVisitor& visitor));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));
    MOCK_METHOD1(pushEvent, void(Event* ev));
    MOCK_METHOD1(handleEvent, void(const Event& ev));

};

class MockVisibleMedia : public MockMedia, public IVisible
{
public:
    MockVisibleMedia(std::unique_ptr<IWidgetAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    IWidgetAdaptor& handler()
    {
        return *m_handler;
    }

    MOCK_METHOD0(show, void());
    MOCK_METHOD0(hide, void());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));

private:
    std::unique_ptr<IWidgetAdaptor> m_handler;

};
