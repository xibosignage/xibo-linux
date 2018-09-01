#pragma once

#include "control/IFixedLayoutWrapper.hpp"

#include <gmock/gmock.h>

class MockFixedLayoutWrapper : public IFixedLayoutWrapper
{
public:
    MOCK_CONST_METHOD0(width, int());
    MOCK_CONST_METHOD0(height, int());
    MOCK_METHOD2(set_size, void(int width, int height));
    MOCK_METHOD3(add_child, void(Gtk::Widget& widget, int left, int top));
    MOCK_METHOD0(show, void());
};
