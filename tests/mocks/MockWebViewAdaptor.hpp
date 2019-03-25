#pragma once

#include "adaptors/IWebView.hpp"
#include "MockWidget.hpp"

#include <gmock/gmock.h>

class MockWebViewAdaptor : public WidgetHandlerAdaptor<IWebViewAdaptor>
{
public:
    MOCK_METHOD0(reload, void());
    MOCK_METHOD1(load, void(const Uri& uri));
    MOCK_METHOD0(enableTransparency, void());
};
