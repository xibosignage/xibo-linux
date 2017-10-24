#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"

#include <iostream>

MainLayout::MainLayout() :
    m_background("back.jpg")
{
    // set_decorated(false);
    // fullscreen();
    // set_keep_above();
    // move(500, 500);

    auto region1 = new Region{Point{420, 420}, Size{150, 150}, 2, 2};
    region1->add_media({std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png")});

    auto region2 = new Region{Point{285, 285}, Size{150, 150}, 2, 2};
    region2->add_media({std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg")});

    auto region3 = new Region{Point{150, 150}, Size{150, 150}, 2, 3};
    region3->add_media({std::make_shared<Image>("3.jpg"),
                       std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg")});

    auto region4 = new Region{Point{15, 15}, Size{150, 150}, 2, 4};
    region4->add_media({std::make_shared<Image>("4.png"),
                       std::make_shared<Image>("1.jpg"),
                       std::make_shared<Image>("2.jpg"),
                       std::make_shared<Image>("3.jpg")});


    auto region5 = new Region{Point{500, 15},  Size{150, 150}, 2, 5};
    region5->add_media(std::make_shared<Image>("4.png"));


    m_mainContainer.add(m_background);
    m_mainContainer.put(*region1, 15, 15);
    m_mainContainer.put(*region2, 150, 150);
    m_mainContainer.put(*region3, 285, 285);
    m_mainContainer.put(*region4, 420, 420);
    m_mainContainer.put(*region5, 500, 15);

    add(m_mainContainer);

    signal_realize().connect(sigc::mem_fun(*this, &MainLayout::on_window_realize));

    m_mainContainer.show();
    m_background.show();
    region1->show();
    region2->show();
    region3->show();
    region4->show();
    region5->show();

}

MainLayout::~MainLayout()
{
}

void MainLayout::on_window_realize()
{
    Gtk::Window::on_realize();

//    auto window = get_window();
//    auto cursor = Gdk::Cursor::create(Gdk::BLANK_CURSOR);
//    window->set_cursor(cursor);
}
