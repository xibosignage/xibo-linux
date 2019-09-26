#pragma once

#include "IGtkWidget.hpp"
#include "IWidget.hpp"

template <typename Interface>
class Widget : public Interface, public IGtkWidget

{
public:
    static_assert(std::is_base_of_v<IWidget, Interface>, "Should implement IWidget");

    Widget(Gtk::Widget& widget) : widget_(widget) {}

    void show() override
    {
        widget_.show();
        signalShown_();
    }

    void showAll() override
    {
        widget_.show();
        signalShown_();
    }

    void hide() override
    {
        widget_.hide();
    }

    bool isShown() const override
    {
        return widget_.is_visible();
    }

    void scale(double scaleX, double scaleY) override
    {
        setSize(static_cast<int>(width() * scaleX), static_cast<int>(height() * scaleY));
    }

    void setSize(int width, int height) override
    {
        widget_.set_size_request(width, height);
    }

    int width() const override
    {
        int width, _;
        widget_.get_size_request(width, _);
        return width;
    }

    int height() const override
    {
        int _, height;
        widget_.get_size_request(_, height);
        return height;
    }

    void setOpacity(double value) override
    {
        widget_.set_opacity(value);
    }

    double opacity() const override
    {
        return widget_.get_opacity();
    }

    SignalShown& shown() override
    {
        return signalShown_;
    }

protected:
    Gtk::Widget& getHandler(IWidget& widget)
    {
        return dynamic_cast<IGtkWidget&>(widget).get();
    }

private:
    Gtk::Widget& widget_;
    SignalShown signalShown_;
};
