#include "GtkDrawingAreaAdaptor.hpp"

GtkDrawingAreaAdaptor::GtkDrawingAreaAdaptor() :
    GtkAdaptor<IDrawingAreaAdaptor>(m_handler)
{
}

Gtk::DrawingArea& GtkDrawingAreaAdaptor::get()
{
    return m_handler;
}
