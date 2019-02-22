#pragma once

#include "MockFixedLayoutAdaptor.hpp"
#include "test_constants.hpp"

#include "control/IRegion.hpp"
#include "control/IRegionContent.hpp"

#include <gmock/gmock.h>

class MockRegion : public IRegion
{
public:
    MockRegion() :
        m_handler(std::make_unique<MockFixedLayoutAdaptor>())
    {
        ON_CALL(*this, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*this, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));
        ON_CALL(*this, handler()).WillByDefault(testing::ReturnRef(*m_handler));
    }

    MOCK_METHOD0(handler, IFixedLayoutAdaptor&());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(loopContent, void());
    MOCK_CONST_METHOD0(contentLooped, bool());
    MOCK_CONST_METHOD0(id, int());
    MOCK_CONST_METHOD0(zorder, int());
    MOCK_METHOD0(show, void());
    MOCK_METHOD3(addContent, void(std::unique_ptr<IRegionContent>&& content, int x, int y));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));

private:
    std::unique_ptr<IFixedLayoutAdaptor> m_handler;

};
