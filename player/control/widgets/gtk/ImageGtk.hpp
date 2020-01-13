#pragma once

#include "control/widgets/Image.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/image.h>

class ImageGtk : public WidgetGtk<Xibo::Image>
{
    static constexpr const int BitsPerSample = 8;
    static constexpr const int DefaultWidth = 8;
    static constexpr const int DefaultHegiht = 8;
    static constexpr const bool DefaultUseAlpha = true;

public:
    struct Error : PlayerRuntimeError
    {
        using PlayerRuntimeError::PlayerRuntimeError;
    };

    ImageGtk();

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    void fillColor(const Color& color) override;
    void loadFrom(const Uri& uri, PreserveRatio preserveRatio) override;

    Gtk::Image& handler() override;

private:
    Glib::RefPtr<const Gdk::Pixbuf> pixbuf() const;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf();

    void check(int width, int height);
    void set(const Glib::RefPtr<Gdk::Pixbuf>& pixbuf);

private:
    Gtk::Image handler_;
};
