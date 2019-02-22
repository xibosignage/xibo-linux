#pragma once

#include "MockOverlayAdaptor.hpp"
#include "test_constants.hpp"

#include "control/IMainLayout.hpp"

#include <gmock/gmock.h>

class MockMainLayout : public IMainLayout
{
public:
    MockMainLayout() :
        m_handler(std::make_unique<MockOverlayAdaptor>())
    {
        ON_CALL(*this, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*this, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));
        ON_CALL(*this, handler()).WillByDefault(testing::ReturnRef(*m_handler));
    }

    MOCK_METHOD0(handler, IOverlayAdaptor&());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD1(setBackground, void(std::unique_ptr<IBackground>&& background));
    MOCK_METHOD3(addRegion, void(std::unique_ptr<IRegion>&& region, int x, int y));
    MOCK_METHOD0(show, void());
    MOCK_METHOD1(handleEvent, void(const Event& ev));
    MOCK_METHOD2(subscribe, void(EventType type, const EventHandler& handler));

private:
    std::unique_ptr<IOverlayAdaptor> m_handler;

};
