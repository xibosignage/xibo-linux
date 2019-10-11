#pragma once

#include "control/widgets/gtk/WidgetGtk.hpp"
#include "control/widgets/render/OutputWindow.hpp"

class OutputWindowGtk : public WidgetGtk<Xibo::OutputWindow>
{
public:
    OutputWindowGtk(Gtk::Widget* handler);

    Gtk::Widget& get() override;

private:
    Gtk::Widget* handler_ = nullptr;
};
