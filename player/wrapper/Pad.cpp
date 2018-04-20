#include "Pad.hpp"

Gst::Pad::Pad(GstPad* handler) :
    m_handler(handler)
{

}

Gst::Pad::~Pad()
{
    g_object_unref(m_handler);
}

Gst::PadLinkReturn Gst::Pad::link(Gst::Pad* other)
{
    return static_cast<PadLinkReturn>(gst_pad_link(m_handler, other->m_handler));
}

GstCaps* Gst::Pad::get_current_caps() const
{
    return gst_pad_get_current_caps(m_handler);
}
