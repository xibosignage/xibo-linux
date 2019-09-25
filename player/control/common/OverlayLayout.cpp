#include "OverlayLayout.hpp"

#include <gtkmm/window.h>

OverlayLayout::OverlayLayout(int width, int height) : Widget(m_handler)
{
    m_handler.signal_get_child_position().connect(sigc::mem_fun(this, &OverlayLayout::onGetChildPosition), false);

    m_handler.property_halign().set_value(Gtk::ALIGN_CENTER);
    m_handler.property_valign().set_value(Gtk::ALIGN_CENTER);

    setSize(width, height);
}

void OverlayLayout::addChild(const std::shared_ptr<IWidget>& child, int x, int y, int z)
{
    m_handler.add_overlay(getHandler(*child));

    WidgetInfo info{child, x, y, z};
    m_children.emplace_back(info);

    sortRegionsByZindex();
    reorderChildren();
}

void OverlayLayout::removeChildren()
{
    for (auto&& child : m_children)
    {
        m_handler.Gtk::Container::remove(getHandler(*child.widget));
    }
    m_children.clear();
}

void OverlayLayout::sortRegionsByZindex()
{
    std::stable_sort(m_children.begin(), m_children.end(),
                     [=](const auto& first, const auto& second) { return first.z < second.z; });
}

void OverlayLayout::reorderChildren()
{
    for (std::size_t i = 0; i != m_children.size(); ++i)
    {
        auto&& widget = m_children[i].widget;
        m_handler.reorder_overlay(getHandler(*widget), static_cast<int>(i));
    }
}

void OverlayLayout::reorderChild(const std::shared_ptr<IWidget>& child, int z)
{
    m_handler.reorder_overlay(getHandler(*child), z);
}

void OverlayLayout::setMainChild(const std::shared_ptr<IWidget>& mainChild)
{
    removePreviousMainChild();

    m_handler.add(getHandler(*mainChild));
    m_mainChild = mainChild;
}

void OverlayLayout::removePreviousMainChild()
{
    if (m_mainChild)
    {
        m_handler.remove();
    }
}

void OverlayLayout::showAll()
{
    Widget::showAll();

    if (m_mainChild)
    {
        m_mainChild->showAll();
    }

    for (auto&& childInfo : m_children)
    {
        childInfo.widget->showAll();
    }
}

void OverlayLayout::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
    //    updateOffsets();
}

void OverlayLayout::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    if (m_mainChild)
    {
        m_mainChild->scale(scaleX, scaleY);
    }
    //    updateOffsets();
    scaleChildren(scaleX, scaleY);
}

void OverlayLayout::updateOffsets()
{
    Gtk::Window* parent = static_cast<Gtk::Window*>(m_handler.get_toplevel());
    if (parent->get_is_toplevel())
    {
        int width, height;
        parent->get_default_size(width, height);

        m_xOffset = (width - this->width()) / 2;
        m_yOffset = (height - this->height()) / 2;
    }
}

void OverlayLayout::scaleChildren(double scaleX, double scaleY)
{
    for (auto&& childInfo : m_children)
    {
        childInfo.widget->scale(scaleX, scaleY);

        childInfo.x *= scaleX;
        childInfo.y *= scaleY;
    }
}

Gtk::Overlay& OverlayLayout::get()
{
    return m_handler;
}

bool OverlayLayout::onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = findChild(widget);
    if (it != m_children.end())
    {
        WidgetInfo info = *it;

        alloc.set_x(m_xOffset + info.x);
        alloc.set_y(m_yOffset + info.y);
        alloc.set_width(info.widget->width());
        alloc.set_height(info.widget->height());
        return true;
    }
    return false;
}

OverlayLayout::WidgetsWithInfo::iterator OverlayLayout::findChild(Gtk::Widget* widget)
{
    return std::find_if(m_children.begin(), m_children.end(),
                        [this, widget](const auto& child) { return &getHandler(*child.widget) == widget; });
}
