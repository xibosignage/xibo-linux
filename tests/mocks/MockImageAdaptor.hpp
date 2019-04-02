#pragma once

#include "adaptors/IImage.hpp"
#include "MockWidget.hpp"

#include <gmock/gmock.h>

class MockImageAdaptor : public WidgetHandlerAdaptor<IImageAdaptor>
{
public:
    MOCK_METHOD1(setColor, void(uint32_t hexColor));
    MOCK_METHOD2(loadImage, void(const Uri& uri, bool preserveAspectRatio));
};
