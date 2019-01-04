#pragma once

#include "IDrawingAreaAdaptor.hpp"
#include "GtkAdaptor.hpp"

#include <gtkmm/drawingarea.h>

class GtkDrawingAreaAdaptor : public GtkAdaptor<IDrawingAreaAdaptor>
{
public:
    GtkDrawingAreaAdaptor();
    Gtk::DrawingArea& get() override;

private:
    Gtk::DrawingArea m_handler;

};
