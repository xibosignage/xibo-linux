#pragma once

#include <cstdint>
#include <utility>

template <typename T>
class StrongType
{
public:
    explicit StrongType() : value_{} {}
    explicit StrongType(const T& value) : value_{value} {}
    explicit StrongType(T&& value) : value_{std::move(value)} {}

    StrongType(const StrongType& other) : value_{other.value_} {}

    StrongType& operator=(const StrongType& other)
    {
        value_ = other.value_;
        return *this;
    }

    StrongType(StrongType&& other) : value_{std::move(other.value_)} {}

    StrongType& operator=(StrongType&& other)
    {
        value_ = std::move(other.value_);
        return *this;
    }

    explicit operator const T&() const noexcept
    {
        return value_;
    }

    explicit operator T&() noexcept
    {
        return value_;
    }

protected:
    T value_;
};
