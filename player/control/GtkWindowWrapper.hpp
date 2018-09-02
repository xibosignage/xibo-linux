#pragma once

#include "IWindowWrapper.hpp"

#include <gtkmm/window.h>

class GtkWindowWrapper : public IWindowWrapper
{
    Gtk::Window m_handler;
public:
    void add(IOverlayWrapper& overlay) override;
    void setDefaultSize(int width, int height) override;
    void move(int x, int y) override;
    void setResizable(bool resizable) override;
    void setDecorated(bool decorated) override;
    void fullscreen() override;
    void setKeepAbove(bool keep_above) override;
    Gtk::Window& get();
};
