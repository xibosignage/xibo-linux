#pragma once

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/fixed.h>

class MyRegion;
class Point;

class MainLayout : public Gtk::Window
{

public:
    MainLayout();
    virtual ~MainLayout();
    void add_to_container(MyRegion* region, const Point& point);

protected:
    void on_window_realize();
    void show_regions();

private:
    void create_ui();

private:
    Gtk::Image m_background;
    Gtk::Fixed m_mainContainer;
    std::vector<MyRegion*> m_regions;

};
