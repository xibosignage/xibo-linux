#include "MainLoop.hpp"

#include "common/logger/Logging.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

static std::thread::id g_uiThreadId;

MainLoop::MainLoop(const std::string& name) : parentApp_(Gtk::Application::create(name))
{
    g_uiThreadId = std::this_thread::get_id();
}

std::thread::id MainLoop::uiThreadId()
{
    return g_uiThreadId;
}

int MainLoop::run(WindowGtk& adaptor)
{
    auto&& windowHandler = adaptor.handler();

    parentApp_->signal_shutdown().connect([this]() {
        idleConnection_.disconnect();
        if (shutdownAction_)
        {
            shutdownAction_();
        }
    });

    parentApp_->signal_startup().connect([this, &windowHandler]() {
        parentApp_->add_window(windowHandler);
        started_();
    });

    return parentApp_->run();
}

void MainLoop::quit()
{
    parentApp_->quit();
}

SignalStarted& MainLoop::started()
{
    return started_;
}

void MainLoop::setShutdownAction(const ShutdownAction& action)
{
    shutdownAction_ = action;
}

void MainLoop::setIdleAction(const IdleAction& action)
{
    idleConnection_ = Glib::MainContext::get_default()->signal_idle().connect(action);
}
