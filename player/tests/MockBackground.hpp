#pragma once

#include "control/IBackground.hpp"

#include <gmock/gmock.h>

class MockBackground : public IBackground
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_CONST_METHOD0(hexColorNumber, uint32_t());
    MOCK_CONST_METHOD0(hexColor, const std::string&());
    MOCK_METHOD1(setColor, void(const std::string& hexColor));
    MOCK_METHOD1(setImage, void(const std::string& imagePath));
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(handler, IImageWrapper&());
};
