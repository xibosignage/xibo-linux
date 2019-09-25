#include "RegionView.hpp"

RegionView::RegionView(int width, int height) : Widget(m_handler)
{
    setSize(width, height);
}

void RegionView::addMedia(const std::shared_ptr<IWidget>& child, int left, int top)
{
    if (child)
    {
        m_handler.put(getHandler(*child), left, top);

        m_media.emplace_back(child);
    }
}

void RegionView::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    scaleMedia(scaleX, scaleY);
}

void RegionView::scaleMedia(double scaleX, double scaleY)
{
    for (auto&& widget : m_media)
    {
        auto&& handler = getHandler(*widget);

        int x = m_handler.child_property_x(handler);
        int y = m_handler.child_property_y(handler);

        widget->scale(scaleX, scaleY);
        m_handler.move(handler, static_cast<int>(x * scaleX), static_cast<int>(y * scaleY));
    }
}

Gtk::Fixed& RegionView::get()
{
    return m_handler;
}
