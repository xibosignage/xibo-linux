#pragma once

#include "IWindowAdaptor.hpp"

#include <gtkmm/window.h>

class GtkWindowAdaptor : public IWindowAdaptor
{
public:
    GtkWindowAdaptor();

    void add(IOverlayAdaptor& overlay) override;
    void setDefaultSize(int width, int height) override;
    void move(int x, int y) override;
    void disableWindowResize() override;
    void disableWindowDecoration() override;
    void setKeepAbove(bool keep_above) override;
    void fullscreen() override;
    void unfullscreen() override;
    void setCursorVisible(bool cursorVisible) override;
    bool isVisible() const override;
    Gtk::Window& get();

private:
    void onRealized();

private:
    Gtk::Window m_handler;
    bool m_cursorVisible = true;

};
