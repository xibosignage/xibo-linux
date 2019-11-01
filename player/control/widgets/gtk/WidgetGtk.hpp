#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "control/widgets/Widget.hpp"

#include <gtkmm/widget.h>

class IWidgetGtk
{
public:
    virtual ~IWidgetGtk() = default;
    virtual Gtk::Widget& get() = 0;
};

template <typename Interface>
class WidgetGtk : public Interface, public IWidgetGtk

{
public:
    static_assert(std::is_base_of_v<Xibo::Widget, Interface>, "Should implement Widget");

    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    WidgetGtk(Gtk::Widget& widget) : widget_(widget) {}

    void show() override
    {
        if (visible()) return;

        widget_.show();
        signalShown_();
    }

    void showAll() override
    {
        if (visible()) return;

        widget_.show_all();
        signalShown_();
    }

    void skipShowAll() override
    {
        widget_.set_no_show_all();
    }

    void hide() override
    {
        widget_.hide();
    }

    bool visible() const override
    {
        return widget_.is_visible();
    }

    void scale(double scaleX, double scaleY) override
    {
        checkScale(scaleX, scaleY);
        setSize(static_cast<int>(width() * scaleX), static_cast<int>(height() * scaleY));
    }

    void setSize(int width, int height) override
    {
        checkSize(width, height);
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
        checkOpacity(value);
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
    Gtk::Widget& getHandler(Xibo::Widget& widget)
    {
        return dynamic_cast<IWidgetGtk&>(widget).get();
    }

    void checkSize(int width, int height)
    {
        if (width < 0 || height < 0) throw WidgetGtk::Error{"WidgetGtk", "Size should be non-negative"};
    }

    void checkScale(double scaleX, double scaleY)
    {
        if (scaleX <= 0.0 || scaleY <= 0.0) throw WidgetGtk::Error{"WidgetGtk", "Scale factor should be positive"};
    }

    void checkOpacity(double opacity)
    {
        if (opacity < 0.0 || opacity > 1.0)
            throw WidgetGtk::Error{"WidgetGtk", "Opacity should be in range [0.0; 1.0]"};
    }

private:
    Gtk::Widget& widget_;
    SignalShown signalShown_;
};
