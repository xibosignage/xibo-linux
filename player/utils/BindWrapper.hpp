#pragma once

#include <functional>

template<int Ind, typename R, typename... Args> struct Helper
{
    using Result = R;
    using RawType = R(*)(Args...);
    using StoredType = std::function<R(Args...)>;
    static StoredType function;
};

template <int Ind, typename R, typename... Args>
typename Helper<Ind, R, Args...>::StoredType Helper<Ind, R, Args...>::function;

template <int Ind, typename R, typename... Args>
typename Helper<Ind, R, Args...>::Result wrapper(Args... args)
{
    return Helper<Ind, R, Args...>::function(args...);
}

template <int Ind, typename R, typename... Args, typename BindResult>
typename Helper<Ind, R, Args...>::RawType get_wrapper(BindResult f)
{
    Helper<Ind, R, Args...>::function = f;
    return wrapper<Ind, R, Args...>;
}
