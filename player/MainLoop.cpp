#include "MainLoop.hpp"

#include "adaptors/GtkWindowAdaptor.hpp"

#include <glibmm/main.h>

MainLoop::MainLoop(const std::string& name) :
    m_parentApp(Gtk::Application::create(name))
{
}

void MainLoop::setIdleAction(std::function<bool()> action)
{
    m_idleConnection = Glib::MainContext::get_default()->signal_idle().connect(action);
}

int MainLoop::run(IWindowAdaptor& adaptor)
{
    auto&& windowHandler = static_cast<GtkWindowAdaptor&>(adaptor).get();

    m_parentApp->signal_shutdown().connect([this](){
        m_idleConnection.disconnect();
    });

    m_parentApp->signal_startup().connect([this, &windowHandler](){
        m_parentApp->add_window(windowHandler);
    });

    return m_parentApp->run();
}

