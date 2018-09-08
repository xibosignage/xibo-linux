#pragma once

#include "adaptors/IWindowAdaptor.hpp"

#include <gmock/gmock.h>

class MockWindowAdaptor : public IWindowAdaptor
{
public:
    MOCK_METHOD1(add, void(IWidgetAdaptor& child));
    MOCK_METHOD2(setDefaultSize, void(int width, int height));
    MOCK_METHOD2(move, void(int x, int y));
    MOCK_METHOD0(disableWindowResize, void());
    MOCK_METHOD0(disableWindowDecoration, void());
    MOCK_METHOD1(setKeepAbove, void(bool cursorVisible));
    MOCK_METHOD0(fullscreen, void());
    MOCK_METHOD0(unfullscreen, void());
    MOCK_METHOD1(setCursorVisible, void(bool keepAbove));
    MOCK_CONST_METHOD0(isVisible, bool());
    MOCK_METHOD1(apply, void(AdaptorVisitor& visitor));
};
