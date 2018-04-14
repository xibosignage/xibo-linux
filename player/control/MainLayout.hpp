#pragma once

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <spdlog/spdlog.h>

#include "LayoutOverlay.hpp"

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

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    double width_scale_factor() const;
    double height_scale_factor() const;

    void add_region(std::unique_ptr<Region> region);
    Region& region(size_t index);
    size_t regions_count() const;
    void show_regions();

private:
    void on_window_realize();
    void create_ui();
    void reorder_regions();

    void set_size(int width, int height);
    void scale_to_monitor_size();
    void set_background_color(uint32_t background_color_hex);
    void set_background_image(const std::string& background_image);

private:
    Gtk::Image m_background;
    LayoutOverlay m_main_overlay;

    int m_schema_version;
    int m_width;
    int m_height;
    std::string m_background_image;
    std::string m_background_color;
    bool is_background_set = false;

    double m_width_scale_factor;
    double m_height_scale_factor;
    std::vector<std::unique_ptr<Region>> m_regions;
    std::shared_ptr<spdlog::logger> m_logger;
};
