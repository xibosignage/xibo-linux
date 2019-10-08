#pragma once

#include <cstdint>
#include <utility>

template <typename T>
class WeakType
{
public:
    WeakType() : value_{} {}
    WeakType(const T& value) : value_{value} {}
    WeakType(T&& value) : value_{std::move(value)} {}

    WeakType(const WeakType& other) : value_{other.value_} {}

    WeakType& operator=(const WeakType& other)
    {
        value_ = other.value_;
        return *this;
    }

    WeakType(WeakType&& other) : value_{std::move(other.value_)} {}

    WeakType& operator=(WeakType&& other)
    {
        value_ = std::move(other.value_);
        return *this;
    }

    operator const T&() const
    {
        return value_;
    }

    operator T&()
    {
        return value_;
    }

protected:
    T value_;
};
