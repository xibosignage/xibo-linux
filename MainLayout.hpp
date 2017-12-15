#pragma once

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/fixed.h>

#include <vector>
#include <memory>

class Region;
class Point;
class Size;
class Transition;

class MainLayout : public Gtk::Window
{
public:
    MainLayout(int schema_version,
               int width,
               int height,
               const std::string& background_image,
               const std::string& background_color);
    ~MainLayout() = default;

    int schema_version() const;
    int width() const;
    int height() const;
    const std::string& background_image() const;
    const std::string& background_color() const;

    void add_region(int id,
                    const Size& size,
                    const Point& pos,
                    int zindex,
                    bool looped,
                    const Transition& transition);
    Region& region(size_t index);
    size_t regions_count() const;
    void show_regions();

    // temp
    const std::vector<std::unique_ptr<Region>>& regions() const { return m_regions; }


protected:
    void on_window_realize();

private:
    void create_ui();

private:
    Gtk::Image m_background;
    Gtk::Fixed m_main_container;

    int m_schema_version;
    int m_width;
    int m_height;
    std::string m_background_image;
    std::string m_background_color;

    std::vector<std::unique_ptr<Region>> m_regions;

};
