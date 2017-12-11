#ifndef DISPATCHER_HPP
#define DISPATCHER_HPP

#include <glibmm/dispatcher.h>
#include <tuple>
#include <iostream>
#include <thread>

template <typename... Args>
class Dispatcher : public Glib::Dispatcher
{
public:
    using Glib::Dispatcher::Dispatcher;
    using Glib::Dispatcher::emit;

    void emit(Args... args)
    {
        m_args = std::make_tuple<Args...>(std::forward<Args>(args)...);
        emit();
    }

    sigc::connection connect(const sigc::slot<void, Args...>& slot)
    {
        Glib::Dispatcher::connect([=](){
            call_signal(std::make_index_sequence<sizeof... (Args)>{});
        });

        return m_signal.connect(slot);
    }

private:
    template <typename T, T... N>
    void call_signal(std::integer_sequence<T, N...>)
    {
        m_signal(std::get<N>(m_args)...);
    }

private:
    sigc::signal<void, Args...> m_signal;
    std::tuple<Args...> m_args;

};

#endif // DISPATCHER_HPP
