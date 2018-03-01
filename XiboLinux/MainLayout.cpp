#include "MainLayout.hpp"
#include "Region.hpp"
#include "XiboApp.hpp"
#include "utilities.hpp"

#include <spdlog/spdlog.h>
#include <boost/filesystem.hpp>

MainLayout::MainLayout(int schema_version,
                       int width,
                       int height,
                       const std::string& background_image,
                       const std::string& background_color) :
    m_schema_version(schema_version),
    m_width(width),
    m_height(height),
    m_background_image(background_image),
    m_background_color(background_color)
{
    set_default_size(width, height);
//    set_decorated(false);
//    fullscreen();
//    set_keep_above();
//    move(500, 500);

    signal_realize().connect(sigc::mem_fun(*this, &MainLayout::on_window_realize));

    auto image_path = XiboApp::example_dir() + "/" + m_background_image;
    if(boost::filesystem::exists(image_path))
    {
        spdlog::get(LOGGER)->debug(image_path);
        try
        {
            auto pixbuf = Gdk::Pixbuf::create_from_file(image_path, width, height);
            m_background.set(pixbuf);
            m_main_overlay.add(m_background);
            m_background.show();
        }
        catch(const Gdk::PixbufError& error)
        {
            spdlog::get(LOGGER)->error("Could set background image: {}", std::string{error.what()});
        }
    }

    if(!m_background_color.empty())
    {
        try
        {
            auto pixbuf = Gdk::Pixbuf::create(Gdk::COLORSPACE_RGB, false, 8, m_width, m_height);
            pixbuf->fill(utilities::to_hex(m_background_color));
            m_background.set(pixbuf);
            m_main_overlay.add(m_background);
            m_background.show();
        }
        catch(const Gdk::PixbufError& error)
        {
            spdlog::get(LOGGER)->error("Could set background color: {}", std::string{error.what()});
        }
    }

    add(m_main_overlay);
    m_main_overlay.show();
}

void MainLayout::add_region(int id,
                            const Size& size,
                            const Point& pos,
                            int zindex,
                            bool looped,
                            const Transition& transition)
{
    m_regions.push_back(std::make_unique<Region>(id, size, pos, zindex, looped, transition));

    auto&& point = m_regions.back()->position();
    m_main_overlay.add_overlay(*m_regions.back(), Gdk::Rectangle(point.left, point.top, size.width, size.height));
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
    spdlog::get(LOGGER)->debug("Reordering");

    std::sort(m_regions.begin(), m_regions.end(), [=](const auto& first, const auto& second){
        return first->zindex() < second->zindex();
    });

    for(size_t i = 0; i != regions_count(); ++i)
    {
        spdlog::get(LOGGER)->debug("zindex: {} id: {} order: {}", m_regions[i]->zindex(), m_regions[i]->id(), i);
        m_main_overlay.reorder_overlay(*m_regions[i], static_cast<int>(i));
    }
}

void MainLayout::show_regions()
{
    reorder_regions();

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

int MainLayout::schema_version() const
{
    return m_schema_version;
}

int MainLayout::width() const
{
    return m_width;
}

int MainLayout::height() const
{
    return m_height;
}

const std::string& MainLayout::background_image() const
{
    return m_background_image;
}

const std::string& MainLayout::background_color() const
{
    return m_background_color;
}
