#include <gtkmm/application.h>

#include "MainWindowController.hpp"
#include "common/logger/Logger.hpp"
#include "config.hpp"
#include "constants.hpp"

#include <spdlog/sinks/stdout_sinks.h>

std::vector<spdlog::sink_ptr> createLoggerSinks()
{
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
    return sinks;
}

int main(int /*argc*/, char** /*argv*/)
{
    auto app = Gtk::Application::create();
    auto ui = Gtk::Builder::create_from_file(ProjectResources::uiFile());

    auto logger = XiboLogger::create(SpdLogger, createLoggerSinks());
    logger->setLevel(LoggingLevel::Debug);
    logger->setPattern("[%H:%M:%S.%e] [%t] [%l]: %v");

    Gtk::Window* mainWindow;
    ui->get_widget(Resources::Ui::MainWindow, mainWindow);

    MainWindowController controller{mainWindow, ui};

    mainWindow->show_all();

    return app->run(*mainWindow);
}
