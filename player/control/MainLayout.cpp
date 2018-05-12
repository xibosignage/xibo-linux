#include "MainLayout.hpp"
#include "Region.hpp"
#include "XiboApp.hpp"
#include "IBackground.hpp"

#include <spdlog/spdlog.h>

MainLayout::MainLayout(int schema_version,
                       int width,
                       int height) :
    m_schema_version(schema_version)
{
    m_logger = spdlog::get(LOGGER);

    set_size(width, height);
    set_resizable(false);
//    set_decorated(false);
//    fullscreen();
//    set_keep_above();
//    move(500, 500);

    signal_realize().connect(sigc::mem_fun(*this, &MainLayout::on_window_realize));

    add(m_main_overlay);
    m_main_overlay.show();
}

void MainLayout::add_region(std::unique_ptr<Region> region)
{
    m_regions.push_back(std::move(region));

    auto&& cur_region = m_regions.back();
    Gdk::Rectangle allocation(cur_region->position().left, cur_region->position().top,
                              cur_region->size().width, cur_region->size().height);

    m_main_overlay.add_overlay(*m_regions.back(), allocation);
}

Region& MainLayout::region(size_t index)
{
    return *m_regions.at(index);
}

size_t MainLayout::regions_count() const
{
    return m_regions.size();
}

void MainLayout::reorder_regions()
{
    m_logger->debug("Reordering");

    std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zindex() < second->zindex();
    });

    for(size_t i = 0; i != regions_count(); ++i)
    {
        m_logger->debug("zindex: {} id: {} order: {}", m_regions[i]->zindex(), m_regions[i]->id(), i);
        m_main_overlay.reorder_overlay(*m_regions[i], static_cast<int>(i));
    }
}

void MainLayout::scale_to_monitor_size(std::unique_ptr<IMonitor> monitor)
{
    auto area = monitor->get_area();

    m_width_scale_factor = area.get_width() / static_cast<double>(m_width);
    m_height_scale_factor = area.get_height() / static_cast<double>(m_height);

    set_size(static_cast<int>(m_width * m_width_scale_factor), static_cast<int>(m_height * m_height_scale_factor));

    m_logger->debug("m: {} {} {} {}", area.get_width(), area.get_height(), m_width_scale_factor, m_height_scale_factor);
}

void MainLayout::set_background(std::unique_ptr<IBackground> background)
{
    m_background = std::move(background);
    m_main_overlay.add(*m_background);
    // FIXME: remove previous background
}

void MainLayout::set_size(int width, int height)
{
    m_width = width;
    m_height = height;
    set_default_size(width, height);
    // FIXME: set size for background
}

void MainLayout::show_regions()
{
    reorder_regions();

    m_background->show();
    for(auto&& region : m_regions)
    {
        region->show();
    }
}

void MainLayout::on_window_realize()
{
    Gtk::Window::on_realize();

//    auto window = get_window();
//    auto cursor = Gdk::Cursor::create(Gdk::BLANK_CURSOR);
//    window->set_cursor(cursor);
}

double MainLayout::width_scale_factor() const
{
    return m_width_scale_factor;
}

double MainLayout::height_scale_factor() const
{
    return m_height_scale_factor;
}

int MainLayout::width() const
{
    return m_width;
}

int MainLayout::height() const
{
    return m_height;
}
