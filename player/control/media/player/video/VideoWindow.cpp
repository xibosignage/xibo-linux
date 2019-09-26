#include "VideoWindow.hpp"

VideoWindow::VideoWindow(int width, int height) : Widget(handler_)
{
    setSize(width, height);

    handler_.signal_draw().connect(sigc::mem_fun(this, &VideoWindow::onWindowDrawn));
}

void VideoWindow::drawFrame(const std::shared_ptr<XiboVideoFrame>& frame)
{
    frames_.push_back(frame);
    handler_.queue_draw();
}

Gtk::DrawingArea& VideoWindow::get()
{
    return handler_;
}

bool VideoWindow::onWindowDrawn(const Cairo::RefPtr<Cairo::Context>& context)
{
    if (!frames_.empty())
    {
        auto frame = frames_.front();
        context->set_source(frame->surface(), 0, 0);
        context->paint();

        frames_.pop_front();
    }
    return false;
}
