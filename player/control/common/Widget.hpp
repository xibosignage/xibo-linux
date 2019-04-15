#pragma once

#include <gtkmm/widget.h>
#include <sigc++/signal.h>

using SignalShown = sigc::signal<void()>;

class Widget
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

    SignalShown shown();
    virtual Gtk::Widget& get() = 0;

private:
    Gtk::Widget& m_widget;
    SignalShown m_shown;

};

