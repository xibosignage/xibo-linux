#pragma once

#include <gtkmm/application.h>

class WindowGtk;

using ShutdownAction = std::function<void()>;
using IdleAction = std::function<bool()>;

class MainLoop
{
public:
    MainLoop(const std::string& name);

    int run(WindowGtk& adaptor);
    void quit();

    void setShutdownAction(const ShutdownAction& action);
    void setIdleAction(const IdleAction& action);

private:
    Glib::RefPtr<Gtk::Application> parentApp_;
    sigc::connection idleConnection_;
    ShutdownAction shutdownAction_;
};
