#pragma once

#include <gtkmm/application.h>

class IWindowAdaptor;

class MainLoop
{
public:
    MainLoop(const std::string& name);

    int run(IWindowAdaptor& adaptor);
    void setIdleAction(std::function<bool()> action);

private:
    Glib::RefPtr<Gtk::Application> m_parentApp;
    sigc::connection m_idleConnection;

};
