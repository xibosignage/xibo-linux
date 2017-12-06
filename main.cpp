#include "Image.hpp"
#include "Video.hpp"
#include "Region.hpp"

#include "MainParser.hpp"
#include "MainLayout.hpp"

#include "constants.hpp"

#include <spdlog/spdlog.h>
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    spdlog::stdout_logger_st(LOGGER);
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    auto logger = spdlog::get(LOGGER);
    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.xibo");

    MainParser parser("20.xml");
    auto layout = parser.parse();

    logger->info("Layout; schema_version: {} width: {} height: {}; back_color: {} back_img: {}",
                  layout->schema_version(), layout->width(), layout->height(),
                  layout->background_color(), layout->background_image());

    for(auto&& region : layout->regions())
    {
        logger->info("Region {}; width: {} height: {}; top: {} left: {}; zindex: {} looped: {}",
                      region->id(), region->size().width, region->size().height, region->position().top,
                      region->position().left, region->zindex(), region->looped());

        for(auto&& media : region->medias())
        {
            logger->info("Media {}; render: {} duration: {} uri: {}", media->id(), (int)media->render(), media->duration(), media->uri());
            if(auto image = dynamic_cast<Image*>(media.get()))
            {
                logger->info("Image; scale_type {}; align: {} valign: {}", (int)image->scale_type(), (int)image->align(), (int)image->valign());
            }
            else if(auto video = dynamic_cast<Video*>(media.get()))
            {
                logger->info("Video; muted {}; looped: {}", video->muted(), video->looped());
            }
        }
    }

    layout->show_regions();

    return app->run(*layout);
}
