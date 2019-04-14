#include "VideoScale.hpp"

Gst::VideoScale::VideoScale()
{
    setElement(gst_element_factory_make("videoscale", nullptr));
}

Gst::RefPtr<Gst::VideoScale> Gst::VideoScale::create()
{
    return std::shared_ptr<Gst::VideoScale>(new Gst::VideoScale);
}
