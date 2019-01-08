#pragma once

#include "adaptors/IWindowAdaptor.hpp"
#include "MockWidgetAdaptor.hpp"

#include <gmock/gmock.h>

class MockWindowAdaptor : public WidgetAdaptor<IWindowAdaptor>
{
public:
    MOCK_METHOD1(connectToHandlerResize, void(std::function<void()> handler));
    MOCK_METHOD1(add, void(IWidgetAdaptor& child));
    MOCK_METHOD0(remove, void());
    MOCK_METHOD2(move, void(int x, int y));
    MOCK_METHOD0(disableWindowResize, void());
    MOCK_METHOD0(disableWindowDecoration, void());
    MOCK_METHOD1(setKeepAbove, void(bool cursorVisible));
    MOCK_METHOD0(fullscreen, void());
    MOCK_METHOD0(unfullscreen, void());
    MOCK_METHOD1(setCursorVisible, void(bool keepAbove));
};
