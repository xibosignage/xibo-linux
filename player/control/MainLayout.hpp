#pragma once

#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <spdlog/spdlog.h>

#include "LayoutOverlay.hpp"
#include "IBackground.hpp"
#include "IMonitor.hpp"

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
               int height);
    ~MainLayout() = default;

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    double width_scale_factor() const;
    double height_scale_factor() const;
    int width() const;
    int height() const;

    void scale_to_monitor_size(std::unique_ptr<IMonitor> monitor);
    void set_background(std::unique_ptr<IBackground> background);

    size_t regions_count() const;
    Region& region(size_t index);
    void add_region(std::unique_ptr<Region> region);
    void show_regions();

private:
    void on_window_realize();
    void create_ui();
    void reorder_regions();
    void set_size(int width, int height);

private:
    LayoutOverlay m_main_overlay;

    int m_schema_version;
    int m_width = 0;
    int m_height = 0;
    std::unique_ptr<IBackground> m_background;

    double m_width_scale_factor = 0.0;
    double m_height_scale_factor = 0.0;
    std::vector<std::unique_ptr<Region>> m_regions;
    std::shared_ptr<spdlog::logger> m_logger;
};
