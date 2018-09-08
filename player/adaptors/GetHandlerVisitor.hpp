#pragma once

#include "AdaptorVisitor.hpp"

#include <gtkmm/widget.h>

class GetHandlerVisitor : public AdaptorVisitor
{
public:
    void visit(GtkFixedLayoutAdaptor& widget);
    void visit(GtkImageAdaptor& widget);
    void visit(GtkOverlayAdaptor& widget);
    void visit(GtkWindowAdaptor& widget);

    Gtk::Widget& getHandler();

private:
    Gtk::Widget* m_handler = nullptr;

};
