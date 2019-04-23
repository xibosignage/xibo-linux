#include "VideoConvert.hpp"

Gst::VideoConvert::VideoConvert() :
    Gst::Element("videoconvert")
{
}

Gst::RefPtr<Gst::VideoConvert> Gst::VideoConvert::create()
{
    return std::shared_ptr<Gst::VideoConvert>(new Gst::VideoConvert);
}
