#pragma once

#include <string>

template <typename T>
class Field
{
public:
    explicit Field(std::string_view name) : m_fieldName(name), m_value{} {}

    explicit Field(std::string_view name, const T& defaultValue) : m_fieldName(name), m_value(defaultValue) {}

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
        return m_fieldName;
    }

    void setValue(const T& value)
    {
        m_value = value;
    }

    T value() const
    {
        return m_value;
    }

    operator T() const
    {
        return m_value;
    }

private:
    std::string m_fieldName;
    T m_value;
};
