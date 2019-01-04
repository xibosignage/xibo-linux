#pragma once

#include "adaptors/IOverlayAdaptor.hpp"
#include "MockWidgetAdaptor.hpp"

#include <gmock/gmock.h>

class MockOverlayAdaptor : public WidgetAdaptor<IOverlayAdaptor>
{
public:
    MOCK_METHOD5(addChild, void(IWidgetAdaptor& child, int top, int left, int width, int height));
    MOCK_METHOD0(removeChildren, void());
    MOCK_METHOD1(addMainChild, void(IWidgetAdaptor& mainChild));
    MOCK_METHOD0(removeMainChild, void());
    MOCK_METHOD2(reorderChild, void(IWidgetAdaptor& child, int position));
};
