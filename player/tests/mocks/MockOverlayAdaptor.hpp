#pragma once

#include "adaptors/IOverlayAdaptor.hpp"

#include <gmock/gmock.h>

class MockOverlayAdaptor : public IOverlayAdaptor
{
public:
    MOCK_METHOD0(show, void());
    MOCK_CONST_METHOD0(isShown, bool());
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD5(addChild, void(IWidgetAdaptor& child, int top, int left, int width, int height));
    MOCK_METHOD0(removeChildren, void());
    MOCK_METHOD1(addMainChild, void(IWidgetAdaptor& mainChild));
    MOCK_METHOD0(removeMainChild, void());
    MOCK_METHOD2(reorderChild, void(IWidgetAdaptor& child, int position));
    MOCK_METHOD1(apply, void(AdaptorVisitor& visitor));
};
