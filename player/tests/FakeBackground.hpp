#pragma once

#include "control/IBackground.hpp"
#include <gtkmm/image.h>

struct FakeBackground : IBackground
{
    FakeBackground() = default;
    void set_color(uint32_t) override { }
    void set_image(const boost::filesystem::path&) override { }
    void set_size(int, int) override { set_size_called = true; }
    void show() override { is_shown = true; }
    operator Gtk::Image&() override { return m_handler; }

    bool set_size_called = false;
    bool is_shown = false;
    Gtk::Image m_handler;

};
