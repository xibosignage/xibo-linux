#pragma once

#include "control/widgets/OutputWindow.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

class OutputWindowGtk : public WidgetGtk<Xibo::OutputWindow>
{
public:
    OutputWindowGtk(Gtk::Widget* handler);

    Gtk::Widget& handler() override;

private:
    Gtk::Widget* handler_ = nullptr;
};
