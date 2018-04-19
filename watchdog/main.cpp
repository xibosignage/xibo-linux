#include "MainWindow.hpp"

#include <spdlog/spdlog.h>
#include <gtkmm/application.h>

int main(int argc, char *argv[])
{
    spdlog::stdout_logger_st("LOGGER");
    spdlog::set_level(spdlog::level::debug);
    spdlog::set_pattern("[%H:%M:%S] [%l]: %v");

    auto app = Gtk::Application::create(argc, argv, "org.gtkmm.xibowatchog");

    MainWindow window(app);
    return app->run(window);
}
