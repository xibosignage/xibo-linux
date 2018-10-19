#pragma once

#include "adaptors/IWidgetAdaptor.hpp"

#include <gmock/gmock.h>

template<typename Interface>
class WidgetAdaptor : public Interface
{
public:
    MOCK_METHOD2(scale, void(double scaleX, double scaleY));
    MOCK_METHOD2(setSize, void(int width, int height));
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());

    MOCK_METHOD0(show, void());
    MOCK_METHOD0(hide, void());
    MOCK_CONST_METHOD0(isShown, bool());
};

using MockWidgetAdaptor = WidgetAdaptor<IWidgetAdaptor>;
