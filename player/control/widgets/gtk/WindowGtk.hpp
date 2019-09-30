#pragma once

#include "control/widgets/OverlayLayout.hpp"
#include "control/widgets/Window.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/window.h>

using SignalWindowRealized = boost::signals2::signal<void()>;

class WindowGtk : public WidgetGtk<Xibo::Window>
{
public:
    WindowGtk();

    void showAll() override;
    void scale(double scaleX, double scaleY) override;
    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    int x() const override;
    int y() const override;
    void move(int x, int y) override;

    void setChild(const std::shared_ptr<Xibo::Widget>& child) override;
    void disableWindowResize() override;
    void disableWindowDecoration() override;
    void setKeepAbove(bool keepAbove) override;
    void fullscreen() override;
    void unfullscreen() override;
    bool isFullscreen() const override;
    void setCursorVisible(bool cursorVisible) override;
    void setBackgroundColor(const Color& color) override;

    SignalKeyPressed& keyPressed() override;
    Gtk::Window& get() override;

private:
    void setWindowSize(int width, int height);
    void onRealized();
    Gdk::Rectangle getCurrentMonitorGeometry() const;

private:
    Gtk::Window handler_;
    std::shared_ptr<Xibo::Widget> child_;
    SignalWindowRealized resizeSignal_;
    SignalKeyPressed keyPressed_;
    sigc::connection windowState_;
    int originalWidth_, originalHeight_;
    bool cursorVisible_;
    bool fullscreen_;
};
