#pragma once

#include "control/IOverlayWrapper.hpp"

#include <gmock/gmock.h>

class MockOverlayWrapper : public IOverlayWrapper
{
public:
    MOCK_METHOD0(show, void());
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD5(addChild, void(IFixedLayoutWrapper& child, int top, int left, int width, int height));
    MOCK_METHOD0(removeChildren, void());
    MOCK_METHOD1(addMainChild, void(IImageWrapper& background));
    MOCK_METHOD0(removeMainChild, void());
    MOCK_METHOD2(reorderChild, void(IFixedLayoutWrapper& child, int position));
};
