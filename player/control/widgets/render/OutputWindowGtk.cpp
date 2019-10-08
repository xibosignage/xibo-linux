#include "OutputWindowGtk.hpp"

const int DefaultPos = 0;

OutputWindowGtk::OutputWindowGtk(int width, int height) : WidgetGtk(handler_)
{
    WidgetGtk::setSize(width, height);

    handler_.signal_draw().connect(sigc::mem_fun(this, &OutputWindowGtk::onWindowDrawn));
}

void OutputWindowGtk::drawFrame(const std::shared_ptr<RenderFrame>& frame)
{
    frames_.push_back(frame);
    handler_.queue_draw();
}

Gtk::DrawingArea& OutputWindowGtk::get()
{
    return handler_;
}

bool OutputWindowGtk::onWindowDrawn(const Cairo::RefPtr<Cairo::Context>& context)
{
    if (!frames_.empty())
    {
        auto frame = frames_.front();
        context->set_source(frame->surface(), DefaultPos, DefaultPos);
        context->paint();

        frames_.pop_front();
    }
    return false;
}
