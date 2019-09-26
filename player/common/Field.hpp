#pragma once

#include <string>

template <typename T>
class Field
{
public:
    explicit Field(std::string_view name) : fieldName_(name), value_{} {}

    explicit Field(std::string_view name, const T& defaultValue) : fieldName_(name), value_(defaultValue) {}

    Field& operator=(const T& value)
    {
        setValue(value);
        return *this;
    }

    std::string_view type() const
    {
        if constexpr (std::is_same_v<T, std::string>)
            return "string";
        else if constexpr (std::is_integral_v<T>)
            return "int";
        else if constexpr (std::is_same_v<T, double>)
            return "double";

        return "unsupported";
    }

    std::string_view name() const
    {
        return fieldName_;
    }

    void setValue(const T& value)
    {
        value_ = value;
    }

    T value() const
    {
        return value_;
    }

    operator T() const
    {
        return value_;
    }

private:
    std::string fieldName_;
    T value_;
};
