#include "VideoConvert.hpp"

Gst::VideoConvert::VideoConvert()
{
    setElement(gst_element_factory_make("videoconvert", nullptr));
}

Gst::RefPtr<Gst::VideoConvert> Gst::VideoConvert::create()
{
    return std::shared_ptr<Gst::VideoConvert>(new Gst::VideoConvert);
}
