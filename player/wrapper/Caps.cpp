#include "Caps.hpp"

Gst::Caps::Caps(GstCaps* handler) :
    m_handler(handler)
{
}

#include <spdlog/spdlog.h>
#include "constants.hpp"

Gst::Caps::~Caps()
{
    gst_caps_unref(m_handler);
}

Gst::RefPtr<Gst::Structure> Gst::Caps::get_structure(guint index) const
{
    return std::make_shared<Gst::Structure>(gst_caps_get_structure(m_handler, index));
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
