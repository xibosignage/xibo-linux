#pragma once

#include "IVideoWindow.hpp"
#include "control/common/Widget.hpp"
#include "control/media/MediaGeometry.hpp"

#include "XiboVideoFrame.hpp"

#include <gtkmm/drawingarea.h>

using OnDrawnCallback = std::function<bool(const Cairo::RefPtr<Cairo::Context>&)>;

class VideoWindow : public Widget<IVideoWindow>
{
public:
    VideoWindow(int width, int height);

    void drawFrame(const std::shared_ptr<XiboVideoFrame>& frame) override;

    Gtk::DrawingArea& get() override;

private:
    bool onWindowDrawn(const Cairo::RefPtr<Cairo::Context>& context);

private:
    Gtk::DrawingArea handler_;
    std::deque<std::shared_ptr<XiboVideoFrame>> frames_;
    MediaGeometry::ScaleType scaleType_;
};
