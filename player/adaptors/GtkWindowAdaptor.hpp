#pragma once

#include "IWindowAdaptor.hpp"
#include "GtkAdaptor.hpp"

#include <gtkmm/window.h>

class GtkWindowAdaptor : public GtkAdaptor<IWindowAdaptor>
{
public:
    GtkWindowAdaptor();

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
    void loadDefaultStyle();
    Gdk::Rectangle getCurrentMonitorGeometry() const;

private:
    Gtk::Window m_handler;
    int m_originalWidth;
    int m_originalHeight;
    bool m_cursorVisible = true;

};
