#pragma once

#include "control/IBackground.hpp"

#include <gmock/gmock.h>

class MockBackground : public IBackground
{
public:
    MockBackground(std::unique_ptr<IImageAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(handler, IImageAdaptor&());

private:
    std::unique_ptr<IImageAdaptor> m_handler;

};
