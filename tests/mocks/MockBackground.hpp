#pragma once

#include "test_constants.hpp"
#include "MockImage.hpp"

#include "control/IBackground.hpp"

#include <gmock/gmock.h>

class MockBackground : public IBackground
{
public:
    MockBackground() :
        m_handler(std::make_unique<MockImageAdaptor>())
    {
        ON_CALL(*this, width()).WillByDefault(testing::Return(DEFAULT_WIDTH));
        ON_CALL(*this, height()).WillByDefault(testing::Return(DEFAULT_HEIGHT));
        ON_CALL(*this, handler()).WillByDefault(testing::ReturnRef(*m_handler));
    }

    MOCK_METHOD0(handler, IImageAdaptor&());
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(show, void());

private:
    std::unique_ptr<IImageAdaptor> m_handler;
};
