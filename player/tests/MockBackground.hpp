#pragma once

#include "control/IBackground.hpp"

#include <gmock/gmock.h>

class MockBackground : public IBackground
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_METHOD1(setColor, void(const std::string& hexColor));
    MOCK_METHOD2(setImage, void(const uint8_t* imageData, size_t dataSize));
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(handler, IImageAdaptor&());
};
