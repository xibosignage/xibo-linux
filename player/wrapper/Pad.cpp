#include "Pad.hpp"
#include "Caps.hpp"

Gst::Pad::Pad(GstPad* handler, bool take_ownership) :
    m_handler(handler), m_take_ownership(take_ownership)
{
}

Gst::Pad::~Pad()
{
    if(m_take_ownership)
    {
        g_object_unref(m_handler);
    }
}

Gst::PadLinkReturn Gst::Pad::link(const Gst::RefPtr<Gst::Pad>& other)
{
    return static_cast<PadLinkReturn>(gst_pad_link(m_handler, other->m_handler));
}

Gst::RefPtr<Gst::Caps> Gst::Pad::get_current_caps() const
{
    auto caps = gst_pad_get_current_caps(m_handler);

    if(!caps)
        return nullptr;
    return std::make_shared<Gst::Caps>(caps);
}
