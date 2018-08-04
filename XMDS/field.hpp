#ifndef FIELD_HPP
#define FIELD_HPP

#include <string>

template<typename T>
class field
{
public:
    field(std::string_view name) : m_field_name(name) { }
    field& operator=(const T& value)
    {
        m_value = value;
        return *this;
    }
    std::string type() const
    {
        if constexpr(std::is_same_v<T, std::string>)
            return "string";
        else if constexpr(std::is_same_v<T, int>)
            return "int";
        else if constexpr(std::is_same_v<T, double>)
            return "double";

        return "unsupported";
    }
    std::string name() const
    {
        return m_field_name;
    }
    void set_value(const T& value)
    {
        m_value = value;
    }
    T value() const
    {
        return m_value;
    }

private:
    std::string m_field_name;
    T m_value;
};

#endif // FIELD_HPP
