#pragma once

#include "adaptors/IWebViewAdaptor.hpp"
#include "MockWidgetAdaptor.hpp"

#include <gmock/gmock.h>

class MockWebViewAdaptor : public WidgetAdaptor<IWebViewAdaptor>
{
public:
    MOCK_METHOD0(reload, void());
    MOCK_METHOD1(load, void(const FilePath& page));
    MOCK_METHOD0(enableTransparency, void());
};
