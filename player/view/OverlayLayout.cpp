#include "OverlayLayout.hpp"

#include <gtkmm/window.h>

OverlayLayout::OverlayLayout(int width, int height) :
    Widget(m_handler)
{
    m_handler.signal_get_child_position().
            connect(sigc::mem_fun(*this, &OverlayLayout::onGetWidgetPosition), false);

    setSize(width, height);
}

void OverlayLayout::addWidget(const std::shared_ptr<Widget>& child, int x, int y, int z)
{
    m_handler.add_overlay(child->get());
    m_handler.reorder_overlay(child->get(), z);

    WidgetInfo info{child->width(), child->height(), x, y};
    m_children.emplace(std::make_pair(std::move(child), info));
}

void OverlayLayout::removeWidget(const std::shared_ptr<Widget>& child)
{
    auto it = m_children.find(child);
    if(it != m_children.end())
    {
        m_handler.Gtk::Container::remove(child->get());

        m_children.erase(it);
    }
}

void OverlayLayout::removeWidgets()
{
    for(auto&& widget : m_handler.get_children())
    {
        m_handler.Gtk::Container::remove(*widget);
    }
    m_children.clear();
}

void OverlayLayout::addMainWidget(const std::shared_ptr<Widget>& mainChild)
{
    m_handler.add(mainChild->get());

    m_mainChild = mainChild;
}

void OverlayLayout::removeMainWidget()
{
    m_handler.remove();

    m_mainChild.reset();
}

bool OverlayLayout::hasMainWidget() const
{
    return m_mainChild != nullptr;
}

void OverlayLayout::reorderWidget(Widget& child, int position)
{
    auto&& handler = child.get();
    m_handler.reorder_overlay(handler, position);
}

void OverlayLayout::showAll()
{
    Widget::showAll();
    m_mainChild->showAll();

    for(auto&& [child, childInfo] : m_children)
    {
        child->showAll();
    }
}

void OverlayLayout::setSize(int width, int height)
{
    m_handler.set_size_request(width, height);
    updateOffsets();
}

void OverlayLayout::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    m_mainChild->scale(scaleX, scaleY);
    updateOffsets();
    scaleWidgets(scaleX, scaleY);
}

void OverlayLayout::updateOffsets()
{
    Gtk::Window* parent = static_cast<Gtk::Window*>(m_handler.get_toplevel());
    if(parent->get_is_toplevel())
    {
        int width, height;
        parent->get_default_size(width, height);

        m_xOffset = (width - this->width())/2;
        m_yOffset = (height - this->height())/2;
    }
}

void OverlayLayout::scaleWidgets(double scaleX, double scaleY)
{
    for(auto&& [child, childInfo] : m_children)
    {
        child->scale(scaleX, scaleY);

        childInfo.x *= scaleX;
        childInfo.y *= scaleY;
    }
}

Gtk::Overlay& OverlayLayout::get()
{
    return m_handler;
}

bool OverlayLayout::onGetWidgetPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = findWidget(widget);
    if(it != m_children.end())
    {
        WidgetInfo info = it->second;

        alloc.set_x(m_xOffset + info.x);
        alloc.set_y(m_yOffset + info.y);
        alloc.set_width(info.width);
        alloc.set_height(info.height);
        return true;
    }
    return false;
}

OverlayLayout::WidgetsWithInfo::iterator OverlayLayout::findWidget(Gtk::Widget* widget)
{
    return std::find_if(m_children.begin(), m_children.end(), [widget](const auto& value){
        auto&& child = value.first;
        return &child->get() == widget;
    });
}
