#pragma once

#include <boost/signals2/signal.hpp>
#include <string>

template <typename... Args>
class Field
{
    using ValueChanged = boost::signals2::signal<void(const Args&...)>;

public:
    explicit Field() : Field(Args{}...) {}
    explicit Field(const Args&... defaultValue) :
        values_{std::make_tuple(defaultValue...)},
        valueChanged_(std::make_shared<ValueChanged>())
    {
    }
    explicit Field(Args&&... defaultValue) :
        values_{std::make_tuple(std::move(defaultValue)...)},
        valueChanged_(std::make_shared<ValueChanged>())
    {
    }

    Field(const Field&) = default;
    Field& operator=(const Field&) = default;

    void setValue(const Args&... values)
    {
        setValue(std::make_tuple(values...));
    }

    void setValue(const std::tuple<Args...>& values)
    {
        values_ = values;
        tupleValuesChanged(std::make_index_sequence<sizeof...(Args)>{});
    }

    const std::tuple<Args...>& values() const
    {
        return values_;
    }

    template <std::size_t I>
    const auto& value() const
    {
        return std::get<I>(this->values_);
    }

    template <std::size_t I>
    auto& value()
    {
        return std::get<I>(this->values_);
    }

    constexpr std::size_t size() const
    {
        return sizeof...(Args);
    }

    ValueChanged& valueChanged()
    {
        return *valueChanged_;
    }

private:
    template <std::size_t... Is>
    void tupleValuesChanged(std::index_sequence<Is...>)
    {
        (*valueChanged_)(std::get<Is>(values_)...);
    }

protected:
    std::tuple<Args...> values_;
    std::shared_ptr<ValueChanged> valueChanged_;
};

template <typename T>
class Field<T>
{
    using ValueChanged = boost::signals2::signal<void(const T&)>;

public:
    explicit Field() : Field(T{}) {}
    explicit Field(const T& defaultValue) : value_{defaultValue}, valueChanged_(std::make_shared<ValueChanged>()) {}
    explicit Field(T&& defaultValue) : value_{std::move(defaultValue)}, valueChanged_(std::make_shared<ValueChanged>())
    {
    }

    Field(const Field&) = default;
    Field& operator=(const Field&) = default;

    Field& operator=(const T& value)
    {
        setValue(value);
        return *this;
    }
    Field& operator=(T&& value)
    {
        setValue(std::move(value));
        return *this;
    }

    void setValue(const T& value)
    {
        value_ = value;
        (*valueChanged_)(value_);
    }

    void setValue(T&& value)
    {
        value_ = std::move(value);
        (*valueChanged_)(value_);
    }

    T& value()
    {
        return value_;
    }

    const T& value() const
    {
        return value_;
    }

    ValueChanged& valueChanged()
    {
        return *valueChanged_;
    }

    operator T&()
    {
        return value_;
    }

    operator const T&() const
    {
        return value_;
    }

    constexpr std::size_t size() const
    {
        return 1;
    }

private:
    T value_;
    std::shared_ptr<ValueChanged> valueChanged_;
};
