#pragma once

#include <gtkmm/application.h>

class IWindowAdaptor;

using ShutdownAction = std::function<void()>;
using IdleAction = std::function<bool()>;

class MainLoop
{
public:
    MainLoop(const std::string& name);

    int run(IWindowAdaptor& adaptor);

    void setShutdownAction(const ShutdownAction& action);
    void setIdleAction(const IdleAction& action);

private:
    Glib::RefPtr<Gtk::Application> m_parentApp;
    sigc::connection m_idleConnection;
    ShutdownAction m_shutdownAction;

};
