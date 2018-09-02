#pragma once

#include "IFixedLayoutWrapper.hpp"

#include <gtkmm/fixed.h>

class GtkFixedLayoutWrapper : public IFixedLayoutWrapper
{
public:
    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    void addChild(Gtk::Widget& widget, int left, int top) override;
    void show() override;
    Gtk::Fixed& get();

private:
    Gtk::Fixed m_handler;

};
