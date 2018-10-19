#pragma once

#include "adaptors/IFixedLayoutAdaptor.hpp"
#include "MockWidgetAdaptor.hpp"

#include <gmock/gmock.h>

class MockFixedLayoutAdaptor : public WidgetAdaptor<IFixedLayoutAdaptor>
{
public:
    MOCK_METHOD3(addChild, void(IWidgetAdaptor& child, int left, int top));
    MOCK_METHOD0(removeChildren, void());
};
