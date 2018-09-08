#include "GetHandlerVisitor.hpp"

#include "adaptors/GtkFixedLayoutAdaptor.hpp"
#include "adaptors/GtkImageAdaptor.hpp"
#include "adaptors/GtkOverlayAdaptor.hpp"
#include "adaptors/GtkWindowAdaptor.hpp"

void GetHandlerVisitor::visit(GtkFixedLayoutAdaptor& widget)
{
    m_handler = widget.get();
}

void GetHandlerVisitor::visit(GtkImageAdaptor& widget)
{
    m_handler = widget.get();
}

void GetHandlerVisitor::visit(GtkOverlayAdaptor& widget)
{
    m_handler = widget.get();
}

void GetHandlerVisitor::visit(GtkWindowAdaptor& widget)
{
    m_handler = widget.get();
}

Gtk::Widget& GetHandlerVisitor::getHandler()
{
    if(!m_handler)
        throw std::runtime_error("Handler is not set");

    return *m_handler;
}
