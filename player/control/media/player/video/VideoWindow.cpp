#include "VideoWindow.hpp"

VideoWindow::VideoWindow(int width, int height) :
    Widget(m_handler)
{
    setSize(width, height);
}

void VideoWindow::queueDraw()
{
    m_handler.queue_draw();
}

void VideoWindow::setDrawCallback(OnDrawnCallback callback)
{
    m_handler.signal_draw().connect(callback);
}

Gtk::DrawingArea& VideoWindow::get()
{
    return m_handler;
}
