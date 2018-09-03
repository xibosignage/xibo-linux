#pragma once

#include "IWindowAdaptor.hpp"

#include <gtkmm/window.h>

class GtkWindowAdaptor : public IWindowAdaptor
{
public:
    void add(IOverlayAdaptor& overlay) override;
    void setDefaultSize(int width, int height) override;
    void move(int x, int y) override;
    void setResizable(bool resizable) override;
    void setDecorated(bool decorated) override;
    void fullscreen() override;
    void setKeepAbove(bool keep_above) override;
    Gtk::Window& get();

private:
    Gtk::Window m_handler;

};
