#pragma once

#include "control/widgets/Image.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/image.h>

class ImageGtk : public WidgetGtk<Xibo::Image>
{
public:
    struct Error : std::runtime_error
    {
        using std::runtime_error::runtime_error;
    };

    ImageGtk(int width, int height, bool useAlpha);

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;

    void setColor(const Color& color) override;
    void loadFrom(const Uri& uri, PreserveRatio preserveRatio) override;

    Gtk::Image& get() override;

private:
    Glib::RefPtr<const Gdk::Pixbuf> pixbuf() const;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf();

    void checkSize(int width, int height);
    void checkUri(const Uri& uri);

private:
    Gtk::Image handler_;
};
