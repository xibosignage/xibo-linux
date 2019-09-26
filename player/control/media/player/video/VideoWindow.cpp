#include "VideoWindow.hpp"

VideoWindow::VideoWindow(int width, int height) : Widget(m_handler)
{
    setSize(width, height);

    m_handler.signal_draw().connect(sigc::mem_fun(this, &VideoWindow::onWindowDrawn));
}

void VideoWindow::drawFrame(const std::shared_ptr<XiboVideoFrame>& frame)
{
    m_frames.push_back(frame);
    m_handler.queue_draw();
}

Gtk::DrawingArea& VideoWindow::get()
{
    return m_handler;
}

bool VideoWindow::onWindowDrawn(const Cairo::RefPtr<Cairo::Context>& context)
{
    if (!m_frames.empty())
    {
        auto frame = m_frames.front();
        context->set_source(frame->surface(), 0, 0);
        context->paint();

        m_frames.pop_front();
    }
    return false;
}
