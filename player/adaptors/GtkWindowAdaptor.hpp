#pragma once

#include "IWindowAdaptor.hpp"
#include "GtkBaseAdaptor.hpp"

#include <gtkmm/window.h>

class GtkWindowAdaptor : public IWindowAdaptor, public GtkBaseAdaptor
{
public:
    GtkWindowAdaptor();

    void show() override;
    void hide() override;
    bool isShown() const override;

    void scale(double scaleX, double scaleY) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    void add(IWidgetAdaptor& child) override;
    void move(int x, int y) override;
    void disableWindowResize() override;
    void disableWindowDecoration() override;
    void setKeepAbove(bool keep_above) override;
    void fullscreen() override;
    void unfullscreen() override;
    void setCursorVisible(bool cursorVisible) override;
    Gtk::Window& get() override;

private:
    void onRealized();

private:
    Gtk::Window m_handler;
    bool m_cursorVisible = true;

};
