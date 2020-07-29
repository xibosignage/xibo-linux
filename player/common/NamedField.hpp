#pragma once

#include "common/Field.hpp"

template <typename... Args>
class NamedField : public Field<Args...>
{
public:
    template <typename T>
    struct FieldWithName
    {
        FieldWithName(const std::string& name_, const T& value_) : name{name_}, value{value_} {}
        FieldWithName(const std::string& name_) : name{name_}, value{} {}

        std::string name;
        T value;
    };

    explicit NamedField(FieldWithName<Args>&&... fields) :
        Field<Args...>{std::move(fields.value)...},
        names_{std::move(fields.name)...}
    {
    }

    template <std::size_t I>
    const std::string& name() const
    {
        return names_.at(I);
    }

private:
    std::vector<std::string> names_;
};

template <typename T>
class NamedField<T> : public Field<T>
{
public:
    explicit NamedField(std::string_view name) : NamedField{name, T{}} {}
    explicit NamedField(std::string_view name, const T& defaultValue) : Field<T>(defaultValue), fieldName_(name) {}
    explicit NamedField(std::string_view name, T&& defaultValue) : Field<T>(std::move(defaultValue)), fieldName_(name)
    {
    }

    using Field<T>::operator=;

    const std::string& name() const
    {
        return fieldName_;
    }

private:
    std::string fieldName_;
};
