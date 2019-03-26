#include "FixedLayout.hpp"

FixedLayout::FixedLayout(int width, int height) :
    Widget(m_handler)
{
    setSize(width, height);
}

void FixedLayout::addWidget(const std::shared_ptr<Widget>& child, int left, int top)
{
    m_handler.put(child->get(), left, top);

    m_children.emplace_back(child);
}

void FixedLayout::removeWidget(const std::shared_ptr<Widget>& child)
{
    auto it = std::find(m_children.begin(), m_children.end(), child);
    if(it != m_children.end())
    {
        m_handler.remove(child->get());

        m_children.erase(it);
    }
}

Widget& FixedLayout::widget(std::size_t index)
{
    return *m_children[index];
}

void FixedLayout::removeWidgets()
{
    for(auto&& child : m_children)
    {
        m_handler.remove(child->get());
    }

    m_children.clear();
}

void FixedLayout::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    scaleChildren(scaleX, scaleY);
}

void FixedLayout::scaleChildren(double scaleX, double scaleY)
{
    for(auto&& widget : m_children)
    {
        auto&& handler = widget->get();

        int x = m_handler.child_property_x(handler);
        int y = m_handler.child_property_y(handler);

        widget->scale(scaleX, scaleY);
        m_handler.move(handler, static_cast<int>(x * scaleX), static_cast<int>(y * scaleY));
    }
}

Gtk::Fixed& FixedLayout::get()
{
    return m_handler;
}
