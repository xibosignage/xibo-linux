#include "RegionView.hpp"

RegionView::RegionView(int width, int height) : Widget(handler_)
{
    setSize(width, height);
}

void RegionView::addMedia(const std::shared_ptr<IWidget>& child, int left, int top)
{
    if (child)
    {
        handler_.put(getHandler(*child), left, top);

        media_.emplace_back(child);
    }
}

void RegionView::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    scaleMedia(scaleX, scaleY);
}

void RegionView::scaleMedia(double scaleX, double scaleY)
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

Gtk::Fixed& RegionView::get()
{
    return handler_;
}
