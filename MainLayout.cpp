#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"
#include "WebView.hpp"
#include "Video.hpp"
#include "VideoHandler.hpp"

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
    set_default_size(640, 480);
    // set_decorated(false);
    // fullscreen();
    // set_keep_above();
    // move(500, 500);

    signal_realize().connect(sigc::mem_fun(*this, &MainLayout::on_window_realize));
    auto pixbuf = Gdk::Pixbuf::create_from_file("back.jpg", 640, 360);
    m_background.set(pixbuf);
    m_mainContainer.add(m_background);

    add(m_mainContainer);
    m_mainContainer.show();
}

void MainLayout::add_region(const std::shared_ptr<Region>& region)
{
    auto&& point = region->position();
    m_mainContainer.put(*region, point.top, point.left);
    m_regions.push_back(region);
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
