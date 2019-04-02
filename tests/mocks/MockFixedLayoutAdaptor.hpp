#pragma once

#include "adaptors/IRegionView.hpp"
#include "MockWidget.hpp"

#include <gmock/gmock.h>

class MockFixedLayoutAdaptor : public WidgetHandlerAdaptor<IFixedLayoutAdaptor>
{
public:
    MOCK_METHOD3(addChild, void(IWidgetAdaptor& child, int left, int top));
    MOCK_METHOD0(removeChildren, void());
};
