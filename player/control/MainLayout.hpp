#pragma once

#include <gtkmm/image.h>
#include <gtkmm/overlay.h>
#include <spdlog/spdlog.h>

#include "IBackground.hpp"
#include "IMonitor.hpp"
#include "BaseRegion.hpp"

#include <vector>
#include <memory>

class Region;
class Point;
class Size;
class Transition;

class MainLayout : public Gtk::Overlay
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
    int schema_version() const;

    void set_background(std::unique_ptr<IBackground> background);
    IBackground& background();

    void scale_to_monitor_size(const std::shared_ptr<IMonitor>& monitor);
    void set_size(int width, int height);

    size_t regions_count() const;
    BaseRegion& region(size_t index);
    void add_region(std::unique_ptr<BaseRegion> region);
    void show_all();

private:
    void reorder_regions();
    bool on_get_child_position(Gtk::Widget* widget, Gdk::Rectangle& alloc) override;

private:
    int m_schema_version;
    int m_width = 0;
    int m_height = 0;
    std::unique_ptr<IBackground> m_background;

    double m_width_scale_factor = 1.0;
    double m_height_scale_factor = 1.0;
    std::vector<std::unique_ptr<BaseRegion>> m_regions;
    std::shared_ptr<spdlog::logger> m_logger;

};
