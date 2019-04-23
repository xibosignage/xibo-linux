#include "AudioConvert.hpp"

Gst::AudioConvert::AudioConvert() :
    Gst::Element("audioconvert")
{
}

Gst::RefPtr<Gst::AudioConvert> Gst::AudioConvert::create()
{
    return std::shared_ptr<Gst::AudioConvert>(new Gst::AudioConvert);
}
