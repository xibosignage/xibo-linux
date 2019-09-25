#include "VideoConvert.hpp"

Gst::VideoConvert::VideoConvert() : Gst::Element("videoconvert") {}

Gst::RefPtr<Gst::VideoConvert> Gst::VideoConvert::create()
{
    return Gst::RefPtr<Gst::VideoConvert>(new Gst::VideoConvert);
}
