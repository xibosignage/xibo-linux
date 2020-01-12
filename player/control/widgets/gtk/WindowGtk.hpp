#pragma once

#include "control/widgets/SingleContainer.hpp"
#include "control/widgets/Window.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/window.h>

using SingleContainerGtk = SingleContainer<WidgetGtk<Xibo::Window>>;

class WindowGtk : public SingleContainerGtk
{
public:
    WindowGtk();

    void setSize(int width, int height) override;
    int x() const override;
    int y() const override;
    void move(int x, int y) override;

    void disableWindowResize() override;
    void disableWindowDecoration() override;
    void setKeepAbove(bool keepAbove) override;
    void fullscreen() override;
    void unfullscreen() override;
    bool isFullscreen() const override;
    void setCursorVisible(bool cursorVisible) override;
    void setBackgroundColor(const Color& color) override;

    SignalKeyPressed& keyPressed() override;
    Gtk::Window& handler() override;
    NativeWindow nativeWindow() override;

private:
    void onWindowRealized();
    void resizeWindow(int width, int height);
    Gdk::Rectangle currentMonitorGeometry();
    void addToHandler(const std::shared_ptr<Xibo::Widget>& child) override;
    void removeFromHandler(const std::shared_ptr<Xibo::Widget>& child) override;

private:
    Gtk::Window handler_;
    SignalKeyPressed keyPressed_;
    bool cursorVisible_;
    bool fullscreen_;
};
