#include "AudioConvert.hpp"

Gst::AudioConvert::AudioConvert()
{
    m_element = gst_element_factory_make("audioconvert", nullptr);
}

Gst::AudioConvert* Gst::AudioConvert::create()
{
    return new AudioConvert;
}
