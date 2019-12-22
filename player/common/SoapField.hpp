#pragma once

#include "common/NamedField.hpp"

template <typename T>
class SoapField : public NamedField<T>
{
public:
    using NamedField<T>::NamedField;
    using NamedField<T>::operator=;

    // FIXME
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
};
