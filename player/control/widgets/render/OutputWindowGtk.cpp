#include "OutputWindowGtk.hpp"

OutputWindowGtk::OutputWindowGtk(Gtk::Widget* handler) : WidgetGtk(*handler), handler_(handler) {}

Gtk::Widget& OutputWindowGtk::get()
{
    return *handler_;
}
