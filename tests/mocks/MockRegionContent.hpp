#pragma once

#include "MockWidget.hpp"

#include "control/IRegionContent.hpp"
#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockRegionContent : public IRegionContent
{
public:
    MockRegionContent() :
        m_handler(std::make_unique<MockWidgetAdaptor>())
    {
        ON_CALL(*this, handler()).WillByDefault(testing::ReturnRef(*m_handler));
    }

    MOCK_METHOD0(handler, IWidgetAdaptor&());
    MOCK_METHOD0(start, void());
    MOCK_METHOD0(stop, void());
    MOCK_METHOD1(attachMedia, void(std::unique_ptr<IMedia>&& content));
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));

private:
    std::unique_ptr<IWidgetAdaptor> m_handler;

};
