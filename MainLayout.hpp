#pragma once

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/fixed.h>

class MainLayout : public Gtk::Window
{

public:
    MainLayout();
    virtual ~MainLayout();

protected:
    void on_window_realize();

private:
    Gtk::Image m_background;
    Gtk::Fixed m_mainContainer;

};
