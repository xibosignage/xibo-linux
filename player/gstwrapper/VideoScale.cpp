#include "VideoScale.hpp"

Gst::VideoScale::VideoScale() : Gst::Element("videoscale") {}

Gst::RefPtr<Gst::VideoScale> Gst::VideoScale::create()
{
    return Gst::RefPtr<Gst::VideoScale>(new Gst::VideoScale);
}
