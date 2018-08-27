#include "MainLayout.hpp"
#include "BaseRegion.hpp"
#include "IMonitor.hpp"
#include "Background.hpp"
#include "utils/utilities.hpp"

MainLayout::MainLayout(int schema_version,
                       int width,
                       int height) :
    m_schema_version(schema_version)
{
    m_logger = spdlog::get(LOGGER);
    set_size(width, height);

    signal_show().connect(sigc::mem_fun(*this, &MainLayout::on_layout_shown));
}

MainLayout::~MainLayout()
{
}

std::unique_ptr<MainLayout> MainLayout::create(int schemaVersion,
                                               int width,
                                               int height,
                                               const std::string& bgimage,
                                               const std::string& bgcolor)
{
    auto layout = std::make_unique<MainLayout>(schemaVersion, width, height);
    std::unique_ptr<IBackground> background = std::make_unique<Background>(width, height);

    if(!bgcolor.empty())
        background->set_color(utils::to_hex(bgcolor));

    if(!bgimage.empty())
        background->set_image(utils::resources_dir() / bgimage);

    layout->set_background(std::move(background));
    return layout;
}

void MainLayout::add_region(std::unique_ptr<BaseRegion> region)
{
    if(region)
    {
        m_regions.push_back(std::move(region));
        add_overlay(*m_regions.back());
    }
}

BaseRegion& MainLayout::region(size_t index)
{
    return *m_regions.at(index);
}

size_t MainLayout::regions_count() const
{
    return m_regions.size();
}

void MainLayout::scale_to_monitor_size(const std::shared_ptr<IMonitor>& monitor)
{
    if(monitor)
    {
        auto area = monitor->get_area();

        m_width_scale_factor = area.get_width() / static_cast<double>(m_width);
        m_height_scale_factor = area.get_height() / static_cast<double>(m_height);

        set_size(static_cast<int>(m_width * m_width_scale_factor),
                 static_cast<int>(m_height * m_height_scale_factor));

        m_logger->trace("m: {} {} {} {}", area.get_width(), area.get_height(), m_width_scale_factor, m_height_scale_factor);
    }
}

// NOTE: add unit test
bool MainLayout::on_get_child_position(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    int mock1, mock2;
    widget->get_preferred_height(mock1, mock2);
    widget->get_preferred_width(mock1, mock2);

    auto region = dynamic_cast<BaseRegion*>(widget);
    if(region)
    {
        alloc.set_x(region->position().left * m_width_scale_factor);
        alloc.set_y(region->position().top * m_height_scale_factor);
        alloc.set_width(region->size().width);
        alloc.set_height(region->size().height);
        return true;
    }
    return false;
}

void MainLayout::reorder_regions()
{
    std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zindex() < second->zindex();
    });

    for(size_t i = 0; i != regions_count(); ++i)
    {
        m_logger->trace("zindex: {} id: {} order: {}", m_regions[i]->zindex(), m_regions[i]->id(), i);
        reorder_overlay(*m_regions[i], static_cast<int>(i));
    }
}

void MainLayout::on_layout_shown()
{
    reorder_regions();
    if(m_background)
    {
        m_background->show();
    }
    for(auto&& region : m_regions)
    {
        region->show();
    }
}

void MainLayout::set_background(std::unique_ptr<IBackground> background)
{
    // FIXME always true + 100x100 layout and 1920x1080 background = crash
    if(background)
    {
        if(m_background)
        {
            remove();
        }
        m_background = std::move(background);
        m_background->set_size(m_width, m_height);
        add(*m_background);
    }
}

IBackground& MainLayout::background()
{
    return *m_background;
}

void MainLayout::set_size(int width, int height)
{
    m_width = width;
    m_height = height;
    set_size_request(width, height);
    if(m_background)
    {
        m_background->set_size(width, height);
    }
    for(auto&& region : m_regions)
    {
        auto&& size = region->size();
        region->set_size(size.width * m_width_scale_factor, size.height * m_height_scale_factor);
    }
}

int MainLayout::width() const
{
    return m_width;
}

int MainLayout::height() const
{
    return m_height;
}

int MainLayout::schema_version() const
{
    return m_schema_version;
}

double MainLayout::width_scale_factor() const
{
    return m_width_scale_factor;
}

double MainLayout::height_scale_factor() const
{
    return m_height_scale_factor;
}
