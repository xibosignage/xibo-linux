#pragma once

#include "IFixedLayoutAdaptor.hpp"
#include "GtkBaseAdaptor.hpp"

#include <gtkmm/fixed.h>

class GtkFixedLayoutAdaptor : public GtkBaseAdaptor<IFixedLayoutAdaptor>
{
public:
    int width() const override;
    int height() const override;
    void setSize(int width, int height) override;
    void addChild(IWidgetAdaptor& child, int left, int top) override;
    void removeChildren() override;
    void show() override;
    Gtk::Fixed* get() override;
    void apply(AdaptorVisitor& visitor) override;

private:
    Gtk::Fixed m_handler;

};
