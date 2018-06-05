#include "control/MainLayout.hpp"
#include "control/Region.hpp"
#include "control/Background.hpp"
#include "media/Audio.hpp"
#include "media/Video.hpp"
#include "media/WebView.hpp"
#include "media/Image.hpp"
#include "tests/FakeBackground.hpp"
#include "tests/FakeMonitor.hpp"
#include "tests/FakeRegion.hpp"
#include "parsers/CommandLineParser.hpp"
#include "parsers/LayoutParser.hpp"
#include "parsers/RegionParser.hpp"
#include "parsers/ImageParser.hpp"
#include "parsers/VideoParser.hpp"
#include "parsers/AudioParser.hpp"
#include "parsers/WebViewParser.hpp"
#include "utils/utilities.hpp"

#include <fstream>
#include <boost/filesystem/operations.hpp>
#include "gtest/gtest.h"

TEST(utilities, ToHex)
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
    layout.show();

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

TEST(CommandLineParser, Construction)
{
    CommandLineParser parser;
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_FALSE(parser.is_testing());
    EXPECT_FALSE(parser.is_version());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, ParseNoParams)
{
    CommandLineParser parser;
    const char* argv[] = {"./player"};
    bool parse = parser.parse(1, const_cast<char**>(argv));

    ASSERT_FALSE(parse);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_FALSE(parser.is_testing());
    EXPECT_FALSE(parser.is_version());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, ParseNotExistingParam)
{
    CommandLineParser parser;
    const char* argv[] = {"./player", "--asdfasdf"};
    bool parse = parser.parse(2, const_cast<char**>(argv));

    ASSERT_FALSE(parse);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_FALSE(parser.is_testing());
    EXPECT_FALSE(parser.is_version());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}


TEST(CommandLineParser, ParseNotExistingParamWithExist)
{
    CommandLineParser parser;
    const char* argv[] = {"./player", "--unit-testing", "--asdfasdf"};
    bool parse = parser.parse(3, const_cast<char**>(argv));

    ASSERT_FALSE(parse);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_FALSE(parser.is_testing());
    EXPECT_FALSE(parser.is_version());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, Parse1Param)
{
    CommandLineParser parser;
    const char* argv[] = {"./player", "--unit-testing"};
    bool parse = parser.parse(2, const_cast<char**>(argv));

    ASSERT_TRUE(parse);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_TRUE(parser.is_testing());
    EXPECT_FALSE(parser.is_version());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, ParseMultipleParams)
{
    CommandLineParser parser;
    const char* argv[] = {"./player", "--unit-testing", "--version", "--log-level=5"};
    bool parse = parser.parse(4, const_cast<char**>(argv));

    ASSERT_TRUE(parse);
    EXPECT_EQ(spdlog::get(LOGGER)->level(), 5);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_TRUE(parser.is_testing());
    EXPECT_TRUE(parser.is_version());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, ParseNotExistingExampleDir)
{
    CommandLineParser parser;
    std::string testDir = utilities::app_current_dir() + "/FakeDir";

    std::string dir = "--example-dir=" + testDir;
    const char* argv[] = {"./player", dir.c_str()};
    bool parse = parser.parse(2, const_cast<char**>(argv));

    ASSERT_TRUE(parse);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, ParseExampleDirWithoutXlf)
{
    CommandLineParser parser;
    std::string testDir = utilities::app_current_dir() + "/TestDir";

    auto _ = utilities::finally([=](){ boost::filesystem::remove(testDir); });
    bool result = boost::filesystem::create_directory(testDir);
    ASSERT_TRUE(result);

    std::string dir = "--example-dir=" + testDir;
    const char* argv[] = {"./player", dir.c_str()};
    bool parse = parser.parse(2, const_cast<char**>(argv));

    ASSERT_TRUE(parse);
    EXPECT_FALSE(parser.is_example_dir());
    EXPECT_TRUE(parser.example_dir_path().empty());
    EXPECT_TRUE(parser.xlf_path().empty());
}

TEST(CommandLineParser, ParseExampleDirWithXlf)
{
    CommandLineParser parser;
    std::string testDir = utilities::app_current_dir() + "/TestDir";
    std::string xlfFile = testDir + "/test.xlf";

    bool result = boost::filesystem::create_directory(testDir);
    std::ofstream o(xlfFile);

    auto _ = utilities::finally([=, &o](){
        o.close();
        boost::filesystem::remove(xlfFile);
        boost::filesystem::remove(testDir);
    });

    ASSERT_TRUE(result);

    std::string dir = "--example-dir=" + testDir;
    const char* argv[] = {"./player", dir.c_str()};
    bool parse = parser.parse(2, const_cast<char**>(argv));

    ASSERT_TRUE(parse);
    EXPECT_TRUE(parser.is_example_dir());
    EXPECT_EQ(parser.example_dir_path(), testDir);
    EXPECT_EQ(parser.xlf_path(), xlfFile);
}

TEST(LayoutParser, ParseParams)
{
    boost::property_tree::ptree l;
    auto& attrs = l.add_child("<xmlattr>", boost::property_tree::ptree{});
    attrs.add("width", 1920);
    attrs.add("height", 1080);
    attrs.add("bgcolor", "#000000");
    attrs.add("background", "8621.jpg");
    attrs.add("schemaVersion", 3);

    LayoutParser layout_parser(l);
    auto params = layout_parser.parse_params();
    EXPECT_EQ(params.width, 1920);
    EXPECT_EQ(params.height, 1080);
    EXPECT_EQ(params.bgcolor, "#000000");
    EXPECT_EQ(params.bgimage, "8621.jpg");
    EXPECT_EQ(params.schemaVersion, 3);
}

TEST(RegionParser, ParseParams)
{
    boost::property_tree::ptree r;
    auto& attrs = r.add_child("<xmlattr>", boost::property_tree::ptree{});
    auto& options = r.add_child("options", boost::property_tree::ptree{});
    attrs.add("id", 1);
    attrs.add("width", 100.0);
    attrs.add("height", 100.0);
    attrs.add("top", 50.0);
    attrs.add("left", 50.0);
    attrs.add("zindex", 2);
    options.add("loop", false);

    RegionParser region_parser(r);
    auto params = region_parser.parse_params();
    EXPECT_EQ(params.id, 1);
    EXPECT_EQ(params.width, 100);
    EXPECT_EQ(params.height, 100);
    EXPECT_EQ(params.top, 50);
    EXPECT_EQ(params.left, 50);
    EXPECT_EQ(params.zindex, 2);
    EXPECT_EQ(params.loop, false);
}

TEST(LayoutParser, Parse)
{
    boost::property_tree::ptree l;
    auto& attrs = l.add_child("<xmlattr>", boost::property_tree::ptree{});
    attrs.add("width", 1920);
    attrs.add("height", 1080);
    attrs.add("bgcolor", "#000000");
    attrs.add("background", "8621.jpg");
    attrs.add("schemaVersion", 3);

    LayoutParser layout_parser(l);
    auto layout = layout_parser.parse();
    EXPECT_EQ(layout->width(), 1920);
    EXPECT_EQ(layout->height(), 1080);
    EXPECT_EQ(layout->width_scale_factor(), 1.0);
    EXPECT_EQ(layout->height_scale_factor(), 1.0);
    EXPECT_EQ(layout->schema_version(), 3);
    EXPECT_EQ(layout->regions_count(), 0);
}

TEST(RegionParser, Parse)
{
    boost::property_tree::ptree r;
    auto& attrs = r.add_child("<xmlattr>", boost::property_tree::ptree{});
    auto& options = r.add_child("options", boost::property_tree::ptree{});
    attrs.add("id", 1);
    attrs.add("width", 100.0);
    attrs.add("height", 100.0);
    attrs.add("top", 50.0);
    attrs.add("left", 50.0);
    attrs.add("zindex", 2);
    options.add("loop", false);

    RegionParser region_parser(r);
    auto region = region_parser.parse();
    EXPECT_EQ(region->id(), 1);
    EXPECT_EQ(region->size().width, 100);
    EXPECT_EQ(region->size().height, 100);
    EXPECT_EQ(region->position().left, 50);
    EXPECT_EQ(region->position().top, 50);
    EXPECT_EQ(region->zindex(), 2);
    EXPECT_EQ(region->looped(), false);
    EXPECT_EQ(region->looped(), false);
}

TEST(VideoParser, Parse)
{
    boost::property_tree::ptree region;
    auto& r_attrs = region.add_child("<xmlattr>", boost::property_tree::ptree{});
    r_attrs.add("width", 100.0);
    r_attrs.add("height", 100.0);
    boost::property_tree::ptree media;
    auto& attrs = media.add_child("<xmlattr>", boost::property_tree::ptree{});
    auto& options = media.add_child("options", boost::property_tree::ptree{});
    attrs.add("id", 1);
    attrs.add("width", 100);
    attrs.add("height", 100);
    attrs.add("duration", 50.0);
    options.add("uri", "test");
    options.add("loop", true);
    options.add("mute", true);

    VideoParser video_parser(region, media);
    auto video = video_parser.parse();
    auto pvideo = dynamic_cast<Video*>(video.get());
    EXPECT_EQ(pvideo->id(), 1);
    EXPECT_EQ(pvideo->size().width, 100);
    EXPECT_EQ(pvideo->size().height, 100);
    EXPECT_EQ(pvideo->uri(), "/test");
    EXPECT_EQ(pvideo->duration(), 50);
    EXPECT_EQ(pvideo->looped(), true);
    EXPECT_EQ(pvideo->muted(), true);
}

TEST(AudioParser, Parse)
{
    boost::property_tree::ptree region;
    auto& r_attrs = region.add_child("<xmlattr>", boost::property_tree::ptree{});
    r_attrs.add("width", 100.0);
    r_attrs.add("height", 100.0);
    boost::property_tree::ptree media;
    auto& attrs = media.add_child("<xmlattr>", boost::property_tree::ptree{});
    auto& options = media.add_child("options", boost::property_tree::ptree{});
    attrs.add("id", 1);
    attrs.add("duration", 50.0);
    options.add("uri", "test");
    options.add("loop", true);
    options.add("mute", false);
    options.add("volume", 50);

    AudioParser audio_parser(region, media);
    auto audio = audio_parser.parse();
    auto paudio = dynamic_cast<Audio*>(audio.get());
    EXPECT_EQ(paudio->id(), 1);
    EXPECT_EQ(paudio->uri(), "/test");
    EXPECT_EQ(paudio->duration(), 50);
    EXPECT_EQ(paudio->looped(), true);
    EXPECT_EQ(paudio->muted(), false);
    EXPECT_EQ(paudio->volume(), 0.5);
}

TEST(ImageParser, Parse)
{
    boost::property_tree::ptree region;
    auto& r_attrs = region.add_child("<xmlattr>", boost::property_tree::ptree{});
    r_attrs.add("width", 100.0);
    r_attrs.add("height", 100.0);
    boost::property_tree::ptree media;
    auto& attrs = media.add_child("<xmlattr>", boost::property_tree::ptree{});
    auto& options = media.add_child("options", boost::property_tree::ptree{});
    attrs.add("id", 1);
    attrs.add("duration", 50.0);
    attrs.add("width", 100);
    attrs.add("height", 100);
    options.add("uri", "test");
    options.add("scaleType", "center");
    options.add("align", "left");
    options.add("valign", "bottom");

    ImageParser image_parser(region, media);
    auto image = image_parser.parse();
    auto pimage = dynamic_cast<Image*>(image.get());
    EXPECT_EQ(pimage->id(), 1);
    EXPECT_EQ(pimage->uri(), "/test");
    EXPECT_EQ(pimage->size().width, 100);
    EXPECT_EQ(pimage->size().height, 100);
    EXPECT_EQ(pimage->duration(), 50);
    EXPECT_EQ(pimage->scale_type(), Image::ScaleType::Scaled);
    EXPECT_EQ(pimage->align(), Image::Align::Left);
    EXPECT_EQ(pimage->valign(), Image::Valign::Bottom);
}

TEST(WebViewParser, Parse)
{
    boost::property_tree::ptree region;
    auto& r_attrs = region.add_child("<xmlattr>", boost::property_tree::ptree{});
    r_attrs.add("width", 100.0);
    r_attrs.add("height", 100.0);
    boost::property_tree::ptree media;
    auto& attrs = media.add_child("<xmlattr>", boost::property_tree::ptree{});
    auto& options = media.add_child("options", boost::property_tree::ptree{});
    attrs.add("id", 1);
    attrs.add("duration", 50.0);
    attrs.add("width", 100);
    attrs.add("height", 100);
    options.add("uri", "test");
    options.add("transparency", false);
    options.add("modeId", 0);

    WebViewParser webview_parser(region, media);
    auto webview = webview_parser.parse();
    auto pwebview = dynamic_cast<WebView*>(webview.get());
    EXPECT_EQ(pwebview->id(), 1);
    EXPECT_TRUE(pwebview->uri().empty());
    EXPECT_EQ(pwebview->size().width, 100);
    EXPECT_EQ(pwebview->size().height, 100);
    EXPECT_EQ(pwebview->duration(), 50);
    EXPECT_EQ(pwebview->transparent(), false);
    EXPECT_EQ(pwebview->render(), Media::Render::HTML);
}
