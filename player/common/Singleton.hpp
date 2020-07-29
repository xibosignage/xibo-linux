#pragma once

#include <boost/noncopyable.hpp>

template <typename T>
class Singleton : private boost::noncopyable
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }
};
