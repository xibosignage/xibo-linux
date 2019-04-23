#include "VideoScale.hpp"

Gst::VideoScale::VideoScale() :
    Gst::Element("videoscale")
{
}

Gst::RefPtr<Gst::VideoScale> Gst::VideoScale::create()
{
    return std::shared_ptr<Gst::VideoScale>(new Gst::VideoScale);
}
