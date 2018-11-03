#pragma once

#include "control/IBackground.hpp"
#include "constants.hpp"

#include <gmock/gmock.h>
#include <optional>

class MockBackground : public IBackground
{
public:
    MockBackground(std::unique_ptr<IImageAdaptor>&& handler) :
        m_handler(std::move(handler))
    {
    }

    IImageAdaptor& handler()
    {
        return *m_handler;
    }

    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD0(show, void());

private:
    std::unique_ptr<IImageAdaptor> m_handler;
};


class MockOneColorBackground : public MockBackground
{
};


class MockImageBackground : public MockBackground
{
};
