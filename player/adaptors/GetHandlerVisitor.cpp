#include "GetHandlerVisitor.hpp"

#include "adaptors/GtkFixedLayoutAdaptor.hpp"
#include "adaptors/GtkImageAdaptor.hpp"
#include "adaptors/GtkOverlayAdaptor.hpp"
#include "adaptors/GtkWindowAdaptor.hpp"
#include "adaptors/WebKitWebViewAdaptor.hpp"

#include <cassert>

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

void GetHandlerVisitor::visit(WebKitWebViewAdaptor& widget)
{
    m_handler = widget.get();
}

Gtk::Widget& GetHandlerVisitor::getHandler()
{
    assert(m_handler);

    return *m_handler;
}
