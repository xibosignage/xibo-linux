#include "AudioConvert.hpp"

Gst::AudioConvert::AudioConvert() : Gst::Element("audioconvert") {}

Gst::RefPtr<Gst::AudioConvert> Gst::AudioConvert::create()
{
    return Gst::RefPtr<Gst::AudioConvert>(new Gst::AudioConvert);
}
