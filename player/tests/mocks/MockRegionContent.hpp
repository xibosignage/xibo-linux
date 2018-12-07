#pragma once

#include "control/IRegionContent.hpp"
#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockRegionContent : public IRegionContent
{
public:
    MockRegionContent(std::unique_ptr<IWidgetAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    IWidgetAdaptor& handler()
    {
        return *m_handler;
    }

    MOCK_METHOD0(start, void());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD1(attachMedia, void(std::unique_ptr<IMedia>&& content));
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));
    MOCK_METHOD1(pushEvent, void(const Event& ev));

private:
    std::unique_ptr<IWidgetAdaptor> m_handler;

};
