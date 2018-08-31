#pragma once

#include "IWindowWrapper.hpp"

#include <gtkmm/window.h>

class GtkWindowWrapper : public IWindowWrapper
{
    Gtk::Window m_handler;
public:
    void add(IOverlayWrapper& overlay) override;
    void set_default_size(int width, int height) override;
    void move(int x, int y) override;
    void set_resizable(bool resizable) override;
    void set_decorated(bool decorated) override;
    void fullscreen() override;
    void set_keep_above(bool keep_above) override;
    Gtk::Window& get();
};
