#include "OverlayLayout.hpp"

#include <gtkmm/window.h>

OverlayLayout::OverlayLayout(int width, int height) : Widget(handler_)
{
    handler_.signal_get_child_position().connect(sigc::mem_fun(this, &OverlayLayout::onGetChildPosition), false);

    handler_.property_halign().set_value(Gtk::ALIGN_CENTER);
    handler_.property_valign().set_value(Gtk::ALIGN_CENTER);

    setSize(width, height);
}

void OverlayLayout::addChild(const std::shared_ptr<IWidget>& child, int x, int y, int z)
{
    handler_.add_overlay(getHandler(*child));

    WidgetInfo info{child, x, y, z};
    children_.emplace_back(info);

    sortRegionsByZindex();
    reorderChildren();
}

void OverlayLayout::removeChildren()
{
    for (auto&& child : children_)
    {
        handler_.Gtk::Container::remove(getHandler(*child.widget));
    }
    children_.clear();
}

void OverlayLayout::sortRegionsByZindex()
{
    std::stable_sort(children_.begin(), children_.end(),
                     [=](const auto& first, const auto& second) { return first.z < second.z; });
}

void OverlayLayout::reorderChildren()
{
    for (std::size_t i = 0; i != children_.size(); ++i)
    {
        auto&& widget = children_[i].widget;
        handler_.reorder_overlay(getHandler(*widget), static_cast<int>(i));
    }
}

void OverlayLayout::reorderChild(const std::shared_ptr<IWidget>& child, int z)
{
    handler_.reorder_overlay(getHandler(*child), z);
}

void OverlayLayout::setMainChild(const std::shared_ptr<IWidget>& mainChild)
{
    removePreviousMainChild();

    handler_.add(getHandler(*mainChild));
    mainChild_ = mainChild;
}

void OverlayLayout::removePreviousMainChild()
{
    if (mainChild_)
    {
        handler_.remove();
    }
}

void OverlayLayout::showAll()
{
    Widget::showAll();

    if (mainChild_)
    {
        mainChild_->showAll();
    }

    for (auto&& childInfo : children_)
    {
        childInfo.widget->showAll();
    }
}

void OverlayLayout::setSize(int width, int height)
{
    handler_.set_size_request(width, height);
    //    updateOffsets();
}

void OverlayLayout::scale(double scaleX, double scaleY)
{
    Widget::scale(scaleX, scaleY);
    if (mainChild_)
    {
        mainChild_->scale(scaleX, scaleY);
    }
    //    updateOffsets();
    scaleChildren(scaleX, scaleY);
}

void OverlayLayout::updateOffsets()
{
    Gtk::Window* parent = static_cast<Gtk::Window*>(handler_.get_toplevel());
    if (parent->get_is_toplevel())
    {
        int width, height;
        parent->get_default_size(width, height);

        xOffset_ = (width - this->width()) / 2;
        yOffset_ = (height - this->height()) / 2;
    }
}

void OverlayLayout::scaleChildren(double scaleX, double scaleY)
{
    for (auto&& childInfo : children_)
    {
        childInfo.widget->scale(scaleX, scaleY);

        childInfo.x *= scaleX;
        childInfo.y *= scaleY;
    }
}

Gtk::Overlay& OverlayLayout::get()
{
    return handler_;
}

bool OverlayLayout::onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = findChild(widget);
    if (it != children_.end())
    {
        WidgetInfo info = *it;

        alloc.set_x(xOffset_ + info.x);
        alloc.set_y(yOffset_ + info.y);
        alloc.set_width(info.widget->width());
        alloc.set_height(info.widget->height());
        return true;
    }
    return false;
}

OverlayLayout::WidgetsWithInfo::iterator OverlayLayout::findChild(Gtk::Widget* widget)
{
    return std::find_if(children_.begin(), children_.end(),
                        [this, widget](const auto& child) { return &getHandler(*child.widget) == widget; });
}
