#pragma once

#include "control/IImageWrapper.hpp"

#include <gmock/gmock.h>

class MockImageWrapper : public IImageWrapper
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(set_size, void(int width, int height));
    MOCK_METHOD1(set_color, void(uint32_t hex_color));
    MOCK_METHOD1(set_image, void(const std::string& image_path));
    MOCK_METHOD0(show, void());
};
