#pragma once

#include "control/common/Widget.hpp"

#include <gtkmm/drawingarea.h>

using OnDrawnCallback = std::function<bool(const Cairo::RefPtr<Cairo::Context>&)>;

class VideoWindow : public Widget
{
public:
    VideoWindow(int width, int height);

    void queueDraw();
    void setDrawCallback(OnDrawnCallback callback);

    Gtk::DrawingArea& get() override;

private:
    Gtk::DrawingArea m_handler;

};
