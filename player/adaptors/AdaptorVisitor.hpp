#pragma once

class GtkFixedLayoutAdaptor;
class GtkImageAdaptor;
class GtkOverlayAdaptor;
class GtkWindowAdaptor;

class AdaptorVisitor
{
public:
    virtual ~AdaptorVisitor() = default;

    virtual void visit(GtkFixedLayoutAdaptor& widget) = 0;
    virtual void visit(GtkImageAdaptor& widget) = 0;
    virtual void visit(GtkOverlayAdaptor& widget) = 0;
    virtual void visit(GtkWindowAdaptor& widget) = 0;

};
