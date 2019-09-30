#pragma once

#include "control/media/Media.hpp"
#include "control/widgets/render/OutputWindow.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/drawingarea.h>

using OnDrawnCallback = std::function<bool(const Cairo::RefPtr<Cairo::Context>&)>;

class OutputWindowGtk : public WidgetGtk<Xibo::OutputWindow>
{
public:
    OutputWindowGtk(int width, int height);

    void drawFrame(const std::shared_ptr<RenderFrame>& frame) override;
    Gtk::DrawingArea& get() override;

private:
    bool onWindowDrawn(const Cairo::RefPtr<Cairo::Context>& context);

private:
    Gtk::DrawingArea handler_;
    std::deque<std::shared_ptr<RenderFrame>> frames_;
};
