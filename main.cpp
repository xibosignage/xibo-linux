#include <glibmm.h>
#include <gtkmm/window.h>
#include <gtkmm/drawingarea.h>
#include <gtkmm/fixed.h>
#include <gtkmm/button.h>
#include <gtkmm/overlay.h>

#include "constants.hpp"
#include "VideoHandler.hpp"
#include "MainLayout.hpp"
#include "Region.hpp"
#include "Image.hpp"
#include "Video.hpp"
#include <spdlog/spdlog.h>

int main(int argc, char* argv[])
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    auto logger = spdlog::get(LOGGER);
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.xibo");

    MainLayout layout(3, 640, 480, "", "");

    layout.add_region(1, Size{150, 150}, Point{490, 330}, 1, false, Transition{});
    layout.regions().back()->add_media<Image>(1, 1, false, "LayerTest/1.jpg", Image::ScaleType::Center, Image::Align::Center, Image::Valign::Middle);

    layout.add_region(2, Size{150, 150}, Point{400, 285}, 0, false, Transition{});
    layout.regions().back()->add_media<Video>(1, 1, false, "/home/stivius/video.webm", true, false);

    layout.show_regions();

    return app->run(layout);
}
