#pragma once

#include "common/PlayerRuntimeError.hpp"
#include "control/widgets/Widget.hpp"

#include <gtkmm/widget.h>

class IWidgetGtk
{
public:
    virtual ~IWidgetGtk() = default;
    virtual Gtk::Widget& handler() = 0;
};

template <typename Interface>
class WidgetGtk : public Interface, public IWidgetGtk
{
    static_assert(std::is_base_of_v<Xibo::Widget, Interface>, "Should implement Xibo::Widget");

public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    WidgetGtk(Gtk::Widget& handler) : handler_(handler) {}

    void show() override
    {
        if (visible()) return;

        handler_.show();
        shown_();
    }

    void showAll() override
    {
        if (visible()) return;

        handler_.show_all();
        shown_();
    }

    void skipShowAll() override
    {
        handler_.set_no_show_all();
    }

    void hide() override
    {
        handler_.hide();
    }

    bool visible() const override
    {
        return handler_.is_visible();
    }

    void scale(double scaleX, double scaleY) override
    {
        checkScale(scaleX, scaleY);
        setSize(std::round(width() * scaleX), std::round(height() * scaleY));
    }

    void setSize(int width, int height) override
    {
        checkSize(width, height);
        handler_.set_size_request(width, height);
        resized_();
    }

    int width() const override
    {
        int width, _;
        handler_.get_size_request(width, _);
        return width;
    }

    int height() const override
    {
        int _, height;
        handler_.get_size_request(_, height);
        return height;
    }

    void setOpacity(double value) override
    {
        checkOpacity(value);
        handler_.set_opacity(value);
    }

    double opacity() const override
    {
        return handler_.get_opacity();
    }

    SignalShown& shown() override
    {
        return shown_;
    }

    SignalResized& resized() override
    {
        return resized_;
    }

protected:
    Gtk::Widget& handlerFor(const std::shared_ptr<Xibo::Widget>& widget)
    {
        return dynamic_cast<IWidgetGtk&>(*widget).handler();
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
    Gtk::Widget& handler_;
    SignalShown shown_;
    SignalResized resized_;
};
