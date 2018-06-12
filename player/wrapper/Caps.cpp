#include "Caps.hpp"

Gst::Caps::Caps(GstCaps* handler) :
    m_handler(handler)
{
}

Gst::Caps::Caps(const std::string& string_caps) :
    m_string_caps(string_caps),
    m_handler(gst_caps_from_string(m_string_caps.c_str()))
{
}

Gst::Caps::Caps(Gst::Caps&& other)
{
    std::swap(m_handler, other.m_handler);
}

Gst::Caps& Gst::Caps::operator=(Gst::Caps&& other)
{
    std::swap(m_handler, other.m_handler);
    return *this;
}

Gst::Caps::~Caps()
{
    gst_caps_unref(m_handler);
}

Gst::RefPtr<Gst::Caps> Gst::Caps::create(const std::string& string_caps)
{
    return std::make_shared<Gst::Caps>(string_caps);
}

Gst::RefPtr<Gst::Structure> Gst::Caps::get_structure(guint index) const
{
    return std::make_shared<Gst::Structure>(gst_caps_get_structure(m_handler, index));
}

GstCaps* Gst::Caps::get_handler()
{
    return m_handler;
}

Gst::Structure::Structure(GstStructure* handler) :
    m_handler(handler)
{
}

int Gst::Structure::get_height() const
{
    int height;
    gst_structure_get_int(m_handler, "height", &height);
    return height;
}

int Gst::Structure::get_width() const
{
    int width;
    gst_structure_get_int(m_handler, "width", &width);
    return width;
}
