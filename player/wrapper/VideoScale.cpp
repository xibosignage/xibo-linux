#include "VideoScale.hpp"

Gst::VideoScale::VideoScale()
{
    m_element = gst_element_factory_make("videoscale", nullptr);
}

Gst::VideoScale* Gst::VideoScale::create()
{
    return new Gst::VideoScale;
}
