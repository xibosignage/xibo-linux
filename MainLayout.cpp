#include "MainLayout.hpp"
#include "Region.hpp"

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

    if(!m_background_image.empty())
    {
        auto pixbuf = Gdk::Pixbuf::create_from_file(m_background_image, width, height);
        m_background.set(pixbuf);
        m_main_container.add(m_background);
    }

    add(m_main_container);
    m_main_container.show();
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
    m_main_container.add_overlay(*m_regions.back(), Gdk::Rectangle(point.left, point.top, size.width, size.height));
    m_main_container.reorder_overlay(*m_regions.back(), zindex);
}

Region& MainLayout::region(size_t index)
{
    return *m_regions.at(index);
}

size_t MainLayout::regions_count() const
{
    return m_regions.size();
}

void MainLayout::show_regions()
{
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
