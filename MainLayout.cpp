#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"
#include "WebView.hpp"
#include "Video.hpp"
#include "VideoHandler.hpp"

#include <iostream>

MainLayout::MainLayout()
{
    set_default_size(640, 480);
    // set_decorated(false);
    // fullscreen();
    // set_keep_above();
    // move(500, 500);

//    signal_realize().connect(sigc::mem_fun(*this, &MainLayout::on_window_realize));
//    auto pixbuf = Gdk::Pixbuf::create_from_file("back.jpg", 640, 360);
//    m_background.set(pixbuf);
//    m_mainContainer.add(m_background);
    add(m_mainContainer);

    auto region1 = new MyRegion{this, Point{420, 420}, Size{150, 150}, 2, 2};
    region1->add_media({std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png")});

    auto region2 = new MyRegion{this, Point{285, 285}, Size{150, 150}, 2, 2};
    region2->add_media({std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg")});

    auto region3 = new MyRegion{this, Point{150, 150}, Size{150, 150}, 2, 3};
    region3->add_media({std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg")});

    auto region4 = new MyRegion{this, Point{15, 15}, Size{150, 150}, 2, 4};
    region4->add_media({std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg")});


    auto region5 = new MyRegion{this, Point{500, 15},  Size{150, 150}, 2, 5};
    region5->add_media(std::make_shared<Image>("4.png"));

    auto region6 = new MyRegion{this, Point{500, 115},  Size{300, 300}, 2, 5};
    region6->add_media(std::make_shared<WebView>("file:///home/stivius/build-XiboLinux-Desktop_Qt_5_9_2_GCC_64bit-Debug/test.htm"));

    auto region7 = new MyRegion{this, Point{250, 0}, Size{300, 300}, 2, 5};
    region7->add_media(std::make_shared<Video>("", false, false));

    m_mainContainer.show();
    //m_background.show();
    show_regions();
    //show_all();
}

MainLayout::~MainLayout()
{
}

void MainLayout::add_to_container(MyRegion* region, const Point& point)
{
    m_mainContainer.put(*region, point.x, point.y);
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
