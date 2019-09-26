#include "MainLoop.hpp"

#include "control/MainWindow.hpp"

#include <glibmm/main.h>

MainLoop::MainLoop(const std::string& name) : parentApp_(Gtk::Application::create(name)) {}

int MainLoop::run(MainWindow& adaptor)
{
    auto&& windowHandler = adaptor.get();

    parentApp_->signal_shutdown().connect([this]() {
        idleConnection_.disconnect();
        if (shutdownAction_)
        {
            shutdownAction_();
        }
    });

    parentApp_->signal_startup().connect([this, &windowHandler]() { parentApp_->add_window(windowHandler); });

    return parentApp_->run();
}

void MainLoop::quit()
{
    parentApp_->quit();
}

void MainLoop::setShutdownAction(const ShutdownAction& action)
{
    shutdownAction_ = action;
}

void MainLoop::setIdleAction(const IdleAction& action)
{
    idleConnection_ = Glib::MainContext::get_default()->signal_idle().connect(action);
}
