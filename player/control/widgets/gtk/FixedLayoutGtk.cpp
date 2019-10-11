#include "FixedLayoutGtk.hpp"

FixedLayoutGtk::FixedLayoutGtk(int width, int height) : WidgetGtk(handler_)
{
    WidgetGtk::setSize(width, height);
}

void FixedLayoutGtk::addChild(const std::shared_ptr<Xibo::Widget>& child, int left, int top)
{
    if (child)
    {
        handler_.put(getHandler(*child), left, top);

        media_.emplace_back(child);
    }
}

void FixedLayoutGtk::scale(double scaleX, double scaleY)
{
    WidgetGtk::scale(scaleX, scaleY);
    scaleMedia(scaleX, scaleY);
}

void FixedLayoutGtk::scaleMedia(double scaleX, double scaleY)
{
    for (auto&& widget : media_)
    {
        auto&& handler = getHandler(*widget);

        int x = handler_.child_property_x(handler);
        int y = handler_.child_property_y(handler);

        widget->scale(scaleX, scaleY);
        handler_.move(handler, static_cast<int>(x * scaleX), static_cast<int>(y * scaleY));
    }
}

Gtk::Fixed& FixedLayoutGtk::get()
{
    return handler_;
}
