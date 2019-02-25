#pragma once

#include "MockWidgetAdaptor.hpp"
#include "test_constants.hpp"

#include "media/IMedia.hpp"

#include <gmock/gmock.h>

class MockMedia : public IMedia
{
public:
    MOCK_CONST_METHOD0(duration, int());
    MOCK_METHOD1(setDuration, void(int duration));
    MOCK_CONST_METHOD0(id, int());
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));
    MOCK_METHOD1(handleEvent, void(const Event& ev));

};

class MockVisibleMedia : public MockMedia, public IVisible
{
public:
    MockVisibleMedia() :
        m_handler(std::make_unique<MockWidgetAdaptor>())
    {
        ON_CALL(*this, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*this, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));
        ON_CALL(*this, handler()).WillByDefault(testing::ReturnRef(*m_handler));
    }

    MOCK_METHOD0(handler, IWidgetAdaptor&());
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(hide, void());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_CONST_METHOD0(align, MediaGeometry::Align());
    MOCK_CONST_METHOD0(valign, MediaGeometry::Valign());
    MOCK_CONST_METHOD0(scaleType, MediaGeometry::ScaleType());

private:
    std::unique_ptr<IWidgetAdaptor> m_handler;

};
