#include "Caps.hpp"

Gst::Caps::Caps(GstCaps* handler) : handler_(handler) {}

Gst::Caps::Caps(std::string_view stringCaps) : handler_(gst_caps_from_string(stringCaps.data())) {}

Gst::Caps::Caps(Gst::Caps&& other)
{
    std::swap(handler_, other.handler_);
}

Gst::Caps& Gst::Caps::operator=(Gst::Caps&& other)
{
    std::swap(handler_, other.handler_);
    return *this;
}

Gst::Caps::~Caps()
{
    gst_caps_unref(handler_);
}

Gst::RefPtr<Gst::Caps> Gst::Caps::create(std::string_view stringCaps)
{
    return std::make_shared<Gst::Caps>(stringCaps);
}

Gst::RefPtr<Gst::Structure> Gst::Caps::structure(guint index) const
{
    return std::make_shared<Gst::Structure>(gst_caps_get_structure(handler_, index));
}

GstCaps* Gst::Caps::handler()
{
    return handler_;
}

Gst::Structure::Structure(GstStructure* handler) : handler_(handler) {}

int Gst::Structure::getHeight() const
{
    int height;
    gst_structure_get_int(handler_, "height", &height);
    return height;
}

int Gst::Structure::getWidth() const
{
    int width;
    gst_structure_get_int(handler_, "width", &width);
    return width;
}

std::string Gst::Structure::getName() const
{
    return gst_structure_get_name(handler_);
}
