#include <gtkmm/application.h>
#include <spdlog/sinks/stdout_sinks.h>

#include "MainWindowController.hpp"

#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

int main(int /*argc*/, char** /*argv*/)
{
    auto app = Gtk::Application::create();
    auto ui = Gtk::Builder::create_from_file(AppConfig::uiFile().string());

    std::vector<spdlog::sink_ptr> sinks{std::make_shared<spdlog::sinks::stdout_sink_mt>()};
    Log::create(sinks);

    Gtk::Window* mainWindow;
    ui->get_widget(Resources::Ui::MainWindow, mainWindow);

    MainWindowController controller{mainWindow, ui};

    mainWindow->show_all();

    return app->run(*mainWindow);
}
