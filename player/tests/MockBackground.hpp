#pragma once

#include "control/IBackground.hpp"

#include <gmock/gmock.h>

class MockBackground : public IBackground
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(set_size, void(int width, int height));
    MOCK_CONST_METHOD0(hex_color_number, uint32_t());
    MOCK_CONST_METHOD0(hex_color, const std::string&());
    MOCK_METHOD1(set_color, void(const std::string& hex_color));
    MOCK_METHOD1(set_image, void(const std::string& image_path));
    MOCK_METHOD0(show, void());
    MOCK_METHOD0(handler, IImageWrapper&());
};
