#pragma once

#include "IWidgetAdaptor.hpp"
#include "GetHandlerVisitor.hpp"

template<typename Interface>
class GtkBaseAdaptor : public Interface
{
public:
    virtual ~GtkBaseAdaptor() = default;
    virtual Gtk::Widget* get() = 0;

    Gtk::Widget& getHandler(IWidgetAdaptor& child)
    {
        GetHandlerVisitor visitor;
        child.apply(visitor);
        return visitor.getHandler();
    }

};
