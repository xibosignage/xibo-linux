#include "VideoConvert.hpp"

Gst::VideoConvert::VideoConvert()
{
    m_element = gst_element_factory_make("videoconvert", nullptr);
}

Gst::VideoConvert* Gst::VideoConvert::create()
{
    return new Gst::VideoConvert;
}
