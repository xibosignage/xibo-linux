#pragma once

#include <glibmm/main.h>
#include <gtkmm/application.h>
#include <thread>

#include <boost/signals2/signal.hpp>

class WindowGtk;

using ShutdownAction = std::function<void()>;
using IdleAction = std::function<bool()>;
using SignalStarted = boost::signals2::signal<void()>;

class MainLoop
{
public:
    MainLoop(const std::string& name);

    template <typename Callback>
    static void pushToUiThread(Callback callback)
    {
        Glib::MainContext::get_default()->invoke([callback = std::move(callback)] {
            callback();
            return false;
        });
    }

    static std::thread::id uiThreadId();

    int run(WindowGtk& adaptor);
    void quit();
    SignalStarted& started();

    void setShutdownAction(const ShutdownAction& action);
    void setIdleAction(const IdleAction& action);

private:
    Glib::RefPtr<Gtk::Application> parentApp_;
    sigc::connection idleConnection_;
    ShutdownAction shutdownAction_;
    SignalStarted started_;
};

#define CHECK_UI_THREAD() assert(MainLoop::uiThreadId() == std::this_thread::get_id())
