#include "OverlayLayoutGtk.hpp"

#include <gtkmm/window.h>

OverlayLayoutGtk::OverlayLayoutGtk(int width, int height) : WidgetGtk{handler_}
{
    WidgetGtk::setSize(width, height);

    handler_.signal_get_child_position().connect(sigc::mem_fun(this, &OverlayLayoutGtk::onGetChildPosition), false);

    handler_.property_halign().set_value(Gtk::ALIGN_CENTER);
    handler_.property_valign().set_value(Gtk::ALIGN_CENTER);
}

void OverlayLayoutGtk::addChild(const std::shared_ptr<Xibo::Widget>& child, int x, int y, int z)
{
    assert(child);

    handler_.add_overlay(getHandler(*child));
    children_.emplace_back(child, x, y, z);

    sortChildrenByZindex();
    reorderChildren();
}

void OverlayLayoutGtk::sortChildrenByZindex()
{
    std::stable_sort(
        children_.begin(), children_.end(), [=](const auto& first, const auto& second) { return first.z < second.z; });
}

void OverlayLayoutGtk::reorderChildren()
{
    for (size_t i = 0; i != children_.size(); ++i)
    {
        int zorder = static_cast<int>(i);
        auto&& widget = children_[i].widget;

        reorderChild(widget, zorder);
    }
}

void OverlayLayoutGtk::removeChildren()
{
    for (auto&& child : children_)
    {
        handler_.Gtk::Container::remove(getHandler(*child.widget));
    }
    children_.clear();
}

void OverlayLayoutGtk::reorderChild(const std::shared_ptr<Xibo::Widget>& child, int z)
{
    assert(child);

    checkZorder(z);

    handler_.reorder_overlay(getHandler(*child), z);
}

void OverlayLayoutGtk::checkZorder(int z)
{
    if (z < 0) throw OverlayLayoutGtk::Error{"OverlayLayoutGtk", "Zorder should be non-negative"};
}

void OverlayLayoutGtk::setMainChild(const std::shared_ptr<Xibo::Widget>& mainChild)
{
    assert(mainChild);

    removePreviousMainChild();

    handler_.add(getHandler(*mainChild));
    mainChild_ = mainChild;
}

void OverlayLayoutGtk::removePreviousMainChild()
{
    if (mainChild_)
    {
        handler_.remove();
    }
}

void OverlayLayoutGtk::showAll()
{
    WidgetGtk::showAll();

    if (mainChild_)
    {
        mainChild_->showAll();
    }

    for (auto&& childInfo : children_)
    {
        childInfo.widget->showAll();
    }
}

void OverlayLayoutGtk::scale(double scaleX, double scaleY)
{
    WidgetGtk::scale(scaleX, scaleY);
    if (mainChild_)
    {
        mainChild_->scale(scaleX, scaleY);
    }
    scaleChildren(scaleX, scaleY);
}

void OverlayLayoutGtk::scaleChildren(double scaleX, double scaleY)
{
    for (auto&& childInfo : children_)
    {
        childInfo.widget->scale(scaleX, scaleY);

        childInfo.x = static_cast<int>(childInfo.x * scaleX);
        childInfo.y = static_cast<int>(childInfo.y * scaleY);
    }
}

Gtk::Overlay& OverlayLayoutGtk::get()
{
    return handler_;
}

bool OverlayLayoutGtk::onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
{
    auto it = findChild(widget);
    if (it != children_.end())
    {
        ChildInfo info = *it;

        alloc.set_x(info.x);
        alloc.set_y(info.y);
        alloc.set_width(info.widget->width());
        alloc.set_height(info.widget->height());
        return true;
    }
    return false;
}

OverlayLayoutGtk::WidgetsWithInfo::iterator OverlayLayoutGtk::findChild(Gtk::Widget* widget)
{
    return std::find_if(children_.begin(), children_.end(), [this, widget](const auto& child) {
        return &getHandler(*child.widget) == widget;
    });
}
