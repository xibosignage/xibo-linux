#pragma once

#include "Widget.hpp"

#include <gtkmm/drawingarea.h>

using OnDrawnCallback = std::function<bool(const Cairo::RefPtr<Cairo::Context>&)>;

class DrawingArea : public Widget
{
public:
    DrawingArea(int width, int height);

    void queueDraw();
    void setDrawCallback(OnDrawnCallback callback);

    Gtk::DrawingArea& get() override;

private:
    Gtk::DrawingArea m_handler;

};
