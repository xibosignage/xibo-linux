#include "RegionView.hpp"

RegionView::RegionView(int width, int height) :
    Widget(m_handler)
{
    setSize(width, height);
}

void RegionView::addMedia(const std::shared_ptr<Widget>& child, int left, int top)
{
    m_handler.put(child->get(), left, top);

    m_media.emplace_back(child);
}

void RegionView::removeMedia(const std::shared_ptr<Widget>& child)
{
    auto it = std::find(m_media.begin(), m_media.end(), child);
    if(it != m_media.end())
    {
        m_handler.remove(child->get());

        m_media.erase(it);
    }
}

void RegionView::removeAllMedia()
{
    for(auto&& child : m_media)
    {
        m_handler.remove(child->get());
    }

    m_media.clear();
}

void RegionView::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    scaleMedia(scaleX, scaleY);
}

void RegionView::scaleMedia(double scaleX, double scaleY)
{
    for(auto&& widget : m_media)
    {
        auto&& handler = widget->get();

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
