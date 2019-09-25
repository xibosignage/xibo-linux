#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <glibmm/dispatcher.h>
#include <iostream>
#include <thread>
#include <tuple>

template <typename... Args>
class Dispatcher : public Glib::Dispatcher
{
public:
    using Glib::Dispatcher::Dispatcher;

    void emit(Args... args)
    {
        m_args = std::make_tuple<Args...>(std::forward<Args>(args)...);
        Glib::Dispatcher::emit();
    }

    sigc::connection connect(const sigc::slot<void, Args...>& slot)
    {
        Glib::Dispatcher::connect([=]() { invokeSignal(std::make_index_sequence<sizeof...(Args)>{}); });

        return m_signal.connect(slot);
    }

private:
    template <typename T, T... N>
    void invokeSignal(std::integer_sequence<T, N...>)
    {
        m_signal(std::get<N>(m_args)...);
    }

private:
    sigc::signal<void, Args...> m_signal;
    std::tuple<Args...> m_args;
};

#endif  // DISPATCHER_HPP
