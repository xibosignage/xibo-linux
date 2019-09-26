#pragma once

#include <chrono>
#include <glibmm/main.h>
#include <sigc++/connection.h>
#include <type_traits>

class Timer
{
public:
    Timer() = default;
    ~Timer();

    template <typename T, typename Handler>
    void start(std::chrono::duration<int64_t, T> duration, Handler handler)
    {
        stop();

        auto milli = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
        m_timerConnection = Glib::signal_timeout().connect([=]() { return onTimeout(handler); }, milli.count());
    }

    void stop();
    bool active() const;

private:
    template <typename Handler>
    bool onTimeout(Handler handler)
    {
        if constexpr (std::is_same_v<decltype(handler()), void>)
        {
            handler();
            return false;
        }
        else
        {
            return handler();
        }
    }

private:
    sigc::connection m_timerConnection;
};
