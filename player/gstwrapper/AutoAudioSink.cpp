#include "AutoAudioSink.hpp"

Gst::AutoAudioSink::AutoAudioSink() :
    Gst::Element("autoaudiosink")
{
}

Gst::RefPtr<Gst::AutoAudioSink> Gst::AutoAudioSink::create()
{
    return std::shared_ptr<Gst::AutoAudioSink>(new Gst::AutoAudioSink);
}

