#pragma once

#include "control/common/Widget.hpp"

#include <gtkmm/window.h>

using SignalKeyPressed = sigc::signal<void(std::string)>;
using SignalWindowRealized = sigc::signal<void()>;

class MainWindow : public Widget
{
public:
    MainWindow(int width, int height);

    void showAll() override;
    void scale(double scaleX, double scaleY) override;
    void setSize(int width, int height) override;
    int width() const override;
    int height() const override;

    void addWidget(const std::shared_ptr<Widget>& child);
    void removeWidget();
    void move(int x, int y);
    void disableWindowResize();
    void disableWindowDecoration();
    void setKeepAbove(bool keep_above);
    void fullscreen();
    void unfullscreen();
    void setCursorVisible(bool cursorVisible);

    SignalKeyPressed keyPressed();
    Gtk::Window& get() override;

private:
    void setWindowSize(int width, int height);
    void onRealized();
    void loadDefaultStyle();
    Gdk::Rectangle getCurrentMonitorGeometry() const;

private:
    Gtk::Window m_handler;
    std::shared_ptr<Widget> m_child;
    SignalWindowRealized m_resizeSignal;
    SignalKeyPressed m_keyPressed;
    sigc::connection m_windowState;
    int m_originalWidth;
    int m_originalHeight;
    bool m_cursorVisible = true;

};
