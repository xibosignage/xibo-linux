#pragma once

#include "adaptors/IImageAdaptor.hpp"
#include "MockWidgetAdaptor.hpp"

#include <gmock/gmock.h>

class MockImageAdaptor : public WidgetAdaptor<IImageAdaptor>
{
public:
    MOCK_METHOD1(setColor, void(uint32_t hexColor));
    MOCK_METHOD2(loadImage, void(const FilePath& path, bool preserveAspectRatio));
};
