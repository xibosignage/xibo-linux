#include "Capsfilter.hpp"
#include "Caps.hpp"

Gst::Capsfilter::Capsfilter() : Gst::Element("capsfilter") {}

Gst::RefPtr<Gst::Capsfilter> Gst::Capsfilter::create()
{
    return Gst::RefPtr<Gst::Capsfilter>(new Gst::Capsfilter);
}

void Gst::Capsfilter::setCaps(Gst::RefPtr<Gst::Caps> caps)
{
    caps_ = std::move(caps);
    g_object_set(element(), "caps", caps_->handler(), nullptr);
}
