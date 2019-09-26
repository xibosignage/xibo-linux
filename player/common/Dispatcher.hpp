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
        args_ = std::make_tuple<Args...>(std::forward<Args>(args)...);
        Glib::Dispatcher::emit();
    }

    sigc::connection connect(const sigc::slot<void, Args...>& slot)
    {
        Glib::Dispatcher::connect([=]() { invokeSignal(std::make_index_sequence<sizeof...(Args)>{}); });

        return signal_.connect(slot);
    }

private:
    template <typename T, T... N>
    void invokeSignal(std::integer_sequence<T, N...>)
    {
        signal_(std::get<N>(args_)...);
    }

private:
    sigc::signal<void, Args...> signal_;
    std::tuple<Args...> args_;
};

#endif  // DISPATCHER_HPP
