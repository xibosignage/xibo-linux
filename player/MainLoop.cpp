#include "MainLoop.hpp"

#include "control/MainWindow.hpp"

#include <glibmm/main.h>

MainLoop::MainLoop(const std::string& name) : m_parentApp(Gtk::Application::create(name)) {}

int MainLoop::run(MainWindow& adaptor)
{
    auto&& windowHandler = adaptor.get();

    m_parentApp->signal_shutdown().connect([this]() {
        m_idleConnection.disconnect();
        if (m_shutdownAction)
        {
            m_shutdownAction();
        }
    });

    m_parentApp->signal_startup().connect([this, &windowHandler]() { m_parentApp->add_window(windowHandler); });

    return m_parentApp->run();
}

void MainLoop::quit()
{
    m_parentApp->quit();
}

void MainLoop::setShutdownAction(const ShutdownAction& action)
{
    m_shutdownAction = action;
}

void MainLoop::setIdleAction(const IdleAction& action)
{
    m_idleConnection = Glib::MainContext::get_default()->signal_idle().connect(action);
}
