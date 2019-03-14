#pragma once

#include "events/EventPublisher.hpp"

#include <gtkmm/widget.h>

class Widget: public EventPublisher<>
{
public:
    Widget(Gtk::Widget& widget);
    virtual ~Widget() = default;

    virtual void show();
    virtual void showAll();
    virtual void hide();
    virtual bool isShown() const;

    virtual void scale(double scaleX, double scaleY);
    virtual void setSize(int width, int height);
    virtual int width() const;
    virtual int height() const;

    virtual Gtk::Widget& get() = 0;

private:
    Gtk::Widget& m_widget;

};

