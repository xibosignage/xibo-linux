#include "Monitor.hpp"

#if GTKMM_MAJOR_VERSION>=3 && GTKMM_MINOR_VERSION>=23
#include <gdkmm/monitor.h>
#endif

Monitor::Monitor(Gtk::Window& window)
{
#if GTKMM_MAJOR_VERSION>=3 && GTKMM_MINOR_VERSION>=23
    m_handler = window.get_display()->get_monitor_at_window(window.get_screen()->get_active_window());
#else
    m_screen = window.get_screen();
    m_handler = m_screen->get_monitor_at_window(m_screen->get_active_window());
#endif
}

Gdk::Rectangle Monitor::get_area()
{
    Gdk::Rectangle area;
#if GTKMM_MAJOR_VERSION>=3 && GTKMM_MINOR_VERSION>=23
    m_handler->get_geometry(area);
#else
    m_screen->get_monitor_geometry(m_handler, area);
#endif
    return area;
}
