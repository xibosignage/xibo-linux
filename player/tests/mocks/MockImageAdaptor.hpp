#pragma once

#include "adaptors/IImageAdaptor.hpp"

#include <gmock/gmock.h>

class MockImageAdaptor : public IImageAdaptor
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD1(setColor, void(uint32_t hexColor));
    MOCK_METHOD1(setImage, void(const std::string& path));
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(hide, void());
    MOCK_METHOD1(apply, void(AdaptorVisitor& visitor));
};
