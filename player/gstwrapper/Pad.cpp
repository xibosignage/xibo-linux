#include "Pad.hpp"
#include "Caps.hpp"

Gst::Pad::Pad(GstPad* handler, bool takeOwnership) : m_handler(handler), m_takeOwnership(takeOwnership) {}

Gst::Pad::~Pad()
{
    if (m_takeOwnership)
    {
        gst_object_unref(m_handler);
    }
}

Gst::PadLinkReturn Gst::Pad::link(const Gst::RefPtr<Gst::Pad>& other)
{
    return static_cast<PadLinkReturn>(gst_pad_link(m_handler, other->m_handler));
}

bool Gst::Pad::isLinked() const
{
    return gst_pad_is_linked(m_handler);
}

Gst::RefPtr<Gst::Caps> Gst::Pad::getCurrentCaps() const
{
    auto caps = gst_pad_get_current_caps(m_handler);

    if (!caps) return nullptr;
    return std::make_shared<Gst::Caps>(caps);
}

Gst::MediaType Gst::Pad::mediaType() const
{
    auto caps = getCurrentCaps();
    if (caps)
    {
        auto name = caps->structure(0)->getName();

        if (name.find("video") != std::string::npos)
            return MediaType::Video;
        else if (name.find("audio") != std::string::npos)
            return MediaType::Audio;
    }
    return MediaType::Invalid;
}
