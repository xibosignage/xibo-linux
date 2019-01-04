#include "AutoAudioSink.hpp"

Gst::AutoAudioSink::AutoAudioSink()
{
    setElement(gst_element_factory_make("autoaudiosink", nullptr));
}

Gst::RefPtr<Gst::AutoAudioSink> Gst::AutoAudioSink::create()
{
    return std::shared_ptr<Gst::AutoAudioSink>(new Gst::AutoAudioSink);
}

