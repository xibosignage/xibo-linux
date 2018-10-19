#pragma once

#include "IFixedLayoutAdaptor.hpp"
#include "GtkBaseAdaptor.hpp"

#include <gtkmm/fixed.h>

class GtkFixedLayoutAdaptor : public IFixedLayoutAdaptor, public GtkBaseAdaptor
{
public:
    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    void scale(double scaleX, double scaleY) override;

    void show() override;
    void hide() override;
    bool isShown() const override;

    void addChild(IWidgetAdaptor& child, int left, int top) override;
    void removeChildren() override;

    Gtk::Fixed& get() override;

private:
    Gtk::Fixed m_handler;

};
