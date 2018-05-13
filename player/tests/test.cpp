#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "control/Background.hpp"
#include "tests/FakeBackground.hpp"
#include "tests/FakeMonitor.hpp"
#include "tests/FakeRegion.hpp"
#include "utils/utilities.hpp"

#include "gtest/gtest.h"

TEST(Utilities, ToHex)
{
    EXPECT_EQ(255, utilities::to_hex("#000"));
    EXPECT_EQ(255, utilities::to_hex("#000000"));
    EXPECT_EQ(255, utilities::to_hex("#000000FF"));
    EXPECT_EQ(301946367, utilities::to_hex("#1F5"));
    EXPECT_EQ(301946367, utilities::to_hex("#11FF55"));
    EXPECT_EQ(301946367, utilities::to_hex("#11FF55FF"));
}

TEST(MainLayout, Construction)
{
    MainLayout layout(0, 640, 480);

    int width, height;
    layout.get_size_request(width, height);

    EXPECT_EQ(layout.schema_version(), 0);
    EXPECT_EQ(layout.width(), 640);
    EXPECT_EQ(layout.height(), 480);
    EXPECT_EQ(layout.width_scale_factor(), 1.0);
    EXPECT_EQ(layout.height_scale_factor(), 1.0);
    EXPECT_EQ(width, 640);
    EXPECT_EQ(height, 480);
    EXPECT_EQ(layout.regions_count(), 0);
}

TEST(MainLayout, SetBackgroundSetSize)
{
    MainLayout layout(0, 640, 480);

    layout.set_background(std::make_unique<FakeBackground>());
    auto&& background = dynamic_cast<FakeBackground&>(layout.background());
    EXPECT_TRUE(background.set_size_called);

    background.set_size_called = false;
    EXPECT_FALSE(background.set_size_called);

    layout.set_size(200, 200); // check for monitor
    EXPECT_TRUE(background.set_size_called);
    EXPECT_EQ(layout.width(), 200);
    EXPECT_EQ(layout.height(), 200);
}

TEST(MainLayout, Set2BackgroundsInARow)
{
    auto layout = std::make_unique<MainLayout>(0, 640, 480);

    int removed_called_count = 0;
    layout->signal_remove().connect([&removed_called_count](Gtk::Widget*){
        ++removed_called_count;
    });

    int add_called_count = 0;
    layout->signal_add().connect([&add_called_count](Gtk::Widget*){
       ++add_called_count;
    });

    // add and remove should be called twice
    layout->set_background(std::make_unique<FakeBackground>());
    auto&& b1 = dynamic_cast<FakeBackground&>(layout->background());
    EXPECT_TRUE(b1.set_size_called);

    layout->set_background(std::make_unique<FakeBackground>());
    auto&& b2 = dynamic_cast<FakeBackground&>(layout->background());
    EXPECT_TRUE(b2.set_size_called);

    layout.reset();
    EXPECT_EQ(removed_called_count, 2);
    EXPECT_EQ(add_called_count, 2);
}

TEST(MainLayout, ScaleToMonitorSize)
{
    MainLayout layout(0, 640, 480);

    auto monitor = std::make_shared<FakeMonitor>(0, 0, 1280, 720);
    layout.scale_to_monitor_size(nullptr);
    EXPECT_FALSE(monitor->get_area_called);

    layout.scale_to_monitor_size(monitor);
    int width, height;
    layout.get_size_request(width, height);
    EXPECT_TRUE(monitor->get_area_called);
    EXPECT_EQ(layout.width(), 1280);
    EXPECT_EQ(layout.height(), 720);
    EXPECT_EQ(width, 1280);
    EXPECT_EQ(height, 720);
    EXPECT_EQ(layout.width_scale_factor(), 2);
    EXPECT_EQ(layout.height_scale_factor(), 1.5);
}

TEST(MainLayout, AddRegion)
{
    MainLayout layout(0, 640, 480);

    EXPECT_EQ(layout.regions_count(), 0);
    layout.add_region(nullptr);
    EXPECT_EQ(layout.regions_count(), 0);

    layout.add_region(std::make_unique<Region>(0, Size{}, Point{}, 0, false));
    layout.add_region(std::make_unique<Region>(0, Size{}, Point{}, 0, false));
    layout.add_region(std::make_unique<Region>(0, Size{}, Point{}, 0, false));
    layout.add_region(std::make_unique<Region>(0, Size{}, Point{}, 0, false));

    EXPECT_EQ(layout.regions_count(), 4);
    EXPECT_EQ(layout.get_children().size(), 4);
}

TEST(MainLayout, ShowAllReorder)
{
    MainLayout layout(0, 640, 480);

    layout.set_background(std::make_unique<FakeBackground>());
    auto&& b = dynamic_cast<FakeBackground&>(layout.background());
    layout.add_region(std::make_unique<FakeRegion>(10));
    layout.add_region(std::make_unique<FakeRegion>(1));
    layout.add_region(std::make_unique<FakeRegion>(6));
    layout.add_region(std::make_unique<FakeRegion>(2));
    layout.show_all();

    EXPECT_TRUE(b.is_shown);
    for(size_t i = 0; i != layout.regions_count(); ++i)
    {
        auto&& region = dynamic_cast<FakeRegion&>(layout.region(i));
        EXPECT_TRUE(region.is_shown);
        EXPECT_EQ(layout.child_property_index(region), i);
    }
}

TEST(Background, SetColor)
{
    Background back(128, 128);
    Gtk::Image& image = back;
    ASSERT_FALSE(image.get_pixbuf());
    back.set_color(utilities::to_hex("#1F5"));
    auto pixbuf = image.get_pixbuf();
    ASSERT_TRUE(pixbuf);
    EXPECT_EQ(pixbuf->get_width(), 128);
    EXPECT_EQ(pixbuf->get_height(), 128);
    EXPECT_EQ(pixbuf->get_pixels()[0], 17);
    EXPECT_EQ(pixbuf->get_pixels()[1], 255);
    EXPECT_EQ(pixbuf->get_pixels()[2], 85);
    EXPECT_EQ(pixbuf->get_has_alpha(), false);
}

TEST(Background, SetImage)
{
    Background back(128, 128);
    Gtk::Image& image = back;
    ASSERT_FALSE(image.get_pixbuf());
    back.set_image("/home/stivius/XiboLinuxStack/XiboExamples/LayerTest.png"); // FIXME: hardcoded path
    auto pixbuf = image.get_pixbuf();
    ASSERT_TRUE(pixbuf);
    EXPECT_EQ(pixbuf->get_width(), 128);
    EXPECT_EQ(pixbuf->get_height(), 96);
}


TEST(Background, SetSize)
{
    Background back(128, 128);
    Gtk::Image& image = back;
    back.set_color(utilities::to_hex("#1F5"));
    back.set_size(64, 64);
    auto pixbuf = image.get_pixbuf();
    EXPECT_EQ(pixbuf->get_width(), 64);
    EXPECT_EQ(pixbuf->get_height(), 64);
}

