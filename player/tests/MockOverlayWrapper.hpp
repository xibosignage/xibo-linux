#pragma once

#include "control/IOverlayWrapper.hpp"

#include <gmock/gmock.h>

class MockOverlayWrapper : public IOverlayWrapper
{
public:
    MOCK_METHOD0(show, void());
    MOCK_METHOD2(set_size, void(int width, int height));
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD5(add_child, void(IFixedLayoutWrapper& child, int top, int left, int width, int height));
    MOCK_METHOD1(add, void(IImageWrapper& background));
    MOCK_METHOD0(remove, void());
    MOCK_METHOD2(reorder_child, void(IFixedLayoutWrapper& child, int position));
};
