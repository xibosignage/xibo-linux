#pragma once

#include <gtkmm/overlay.h>
#include <spdlog/spdlog.h>

#include <vector>
#include <memory>

class BaseRegion;
class IMonitor;
class IBackground;

class MainLayout : public Gtk::Overlay
{
public:
    MainLayout(int schema_version,
               int width,
               int height);
    ~MainLayout();

    MainLayout(const MainLayout& other) = delete;
    MainLayout& operator=(const MainLayout& other) = delete;

    static std::unique_ptr<MainLayout> create(int schemaVersion,
                                              int width,
                                              int height,
                                              const std::string& bgimage,
                                              const std::string& bgcolor);

    int schema_version() const;

    void set_size(int width, int height);
    int width() const;
    int height() const;

    void set_background(std::unique_ptr<IBackground> background);
    IBackground& background();

    void add_region(std::unique_ptr<BaseRegion> region);
    BaseRegion& region(size_t index);
    size_t regions_count() const;

    void scale_to_monitor_size(const std::shared_ptr<IMonitor>& monitor);
    double width_scale_factor() const;
    double height_scale_factor() const;

private:
    void reorder_regions();
    void on_layout_shown();
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
