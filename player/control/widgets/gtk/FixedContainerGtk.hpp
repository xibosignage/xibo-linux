#pragma once

#include "control/widgets/FixedContainer.hpp"
#include "control/widgets/gtk/WidgetGtk.hpp"

#include <gtkmm/overlay.h>

template <typename Interface>
class BaseFixedContainerGtk : public FixedContainer<WidgetGtk<Interface>>
{
public:
    BaseFixedContainerGtk() : FixedContainer<WidgetGtk<Interface>>{handler_}
    {
        handler_.signal_get_child_position().connect(sigc::mem_fun(this, &BaseFixedContainerGtk::onGetChildPosition),
                                                     false);
    }

    Gtk::Overlay& handler() override
    {
        return handler_;
    }

private:
    void addToHandler(const std::shared_ptr<Xibo::Widget>& child, int /*left*/, int /*top*/, int /*zorder*/) override
    {
        handler_.add_overlay(this->handlerFor(child));
    }

    void removeFromHandler(const std::shared_ptr<Xibo::Widget>& child) override
    {
        handler_.Gtk::Container::remove(this->handlerFor(child));
    }

    void reorderInHandler(const std::shared_ptr<Xibo::Widget>& child, int zorder) override
    {
        handler_.reorder_overlay(this->handlerFor(child), zorder);
    }

    bool onGetChildPosition(Gtk::Widget* widget, Gdk::Rectangle& alloc)
    {
        auto it =
            this->findChild([this, widget](const auto& child) { return &this->handlerFor(child.widget) == widget; });
        if (it != this->children().end())
        {
            auto&& [widget, info] = *it;

            alloc.set_x(info.left);
            alloc.set_y(info.top);
            alloc.set_width(widget->width());
            alloc.set_height(widget->height());
            return true;
        }
        return false;
    }

private:
    Gtk::Overlay handler_;
};

using FixedContainerGtk = BaseFixedContainerGtk<Xibo::FixedContainer>;
