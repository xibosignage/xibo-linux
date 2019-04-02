#include "DrawingArea.hpp"

DrawingArea::DrawingArea(int width, int height) :
    Widget(m_handler)
{
    setSize(width, height);
}

void DrawingArea::queueDraw()
{
    m_handler.queue_draw();
}

void DrawingArea::setDrawCallback(OnDrawnCallback callback)
{
    m_handler.signal_draw().connect(callback);
}

Gtk::DrawingArea& DrawingArea::get()
{
    return m_handler;
}
