#pragma once

#include "control/common/Widget.hpp"
#include "XiboVideoFrame.hpp"

#include <gtkmm/drawingarea.h>

using OnDrawnCallback = std::function<bool(const Cairo::RefPtr<Cairo::Context>&)>;

class VideoWindow : public Widget
{
public:
    VideoWindow(int width, int height);

    void drawFrame(const std::shared_ptr<XiboVideoFrame>& frame);

    Gtk::DrawingArea& get() override;

private:
    bool onWindowDrawn(const Cairo::RefPtr<Cairo::Context>& context);

private:
    Gtk::DrawingArea m_handler;
    std::deque<std::shared_ptr<XiboVideoFrame>> m_frames;

};
