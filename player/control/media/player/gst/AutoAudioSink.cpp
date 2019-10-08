#include "AutoAudioSink.hpp"

Gst::AutoAudioSink::AutoAudioSink() : Gst::Element("autoaudiosink") {}

Gst::RefPtr<Gst::AutoAudioSink> Gst::AutoAudioSink::create()
{
    return Gst::RefPtr<Gst::AutoAudioSink>(new Gst::AutoAudioSink);
}
