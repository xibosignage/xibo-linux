#include "Pad.hpp"
#include "Caps.hpp"

Gst::Pad::Pad(GstPad* handler, bool takeOwnership) : handler_(handler), takeOwnership_(takeOwnership) {}

Gst::Pad::~Pad()
{
    if (takeOwnership_)
    {
        gst_object_unref(handler_);
    }
}

Gst::PadLinkReturn Gst::Pad::link(const Gst::RefPtr<Gst::Pad>& other)
{
    return static_cast<PadLinkReturn>(gst_pad_link(handler_, other->handler_));
}

bool Gst::Pad::isLinked() const
{
    return gst_pad_is_linked(handler_);
}

Gst::RefPtr<Gst::Caps> Gst::Pad::getCurrentCaps() const
{
    auto caps = gst_pad_get_current_caps(handler_);

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
