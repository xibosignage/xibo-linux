#pragma once

#include "IImageAdaptor.hpp"
#include "GtkBaseAdaptor.hpp"

#include <gtkmm/image.h>

class GtkImageAdaptor : public GtkBaseAdaptor<IImageAdaptor>
{
public:
    GtkImageAdaptor();

    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;
    void setColor(uint32_t hex_color_number) override;
    void setImage(const std::string& path) override;
    void show() override;
    void hide() override;

    Gtk::Image* get() override;
    void apply(AdaptorVisitor& visitor) override;

private:
    Glib::RefPtr<const Gdk::Pixbuf> pixbuf() const;
    Glib::RefPtr<Gdk::Pixbuf> pixbuf();

private:
    Gtk::Image m_handler;

};
