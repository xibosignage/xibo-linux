#pragma once

#include "IMonitor.hpp"
#include <gtkmm/window.h>

class Monitor : public IMonitor
{
public:
    Monitor(Gtk::Window& window);
    Gdk::Rectangle get_area() override;

private:
#if GTKMM_MAJOR_VERSION>=3 && GTKMM_MINOR_VERSION>=23
    Glib::RefPtr<Gdk::Monitor> m_handler;
#else
    int m_handler = 0;
    Glib::RefPtr<Gdk::Screen> m_screen;
#endif
};
