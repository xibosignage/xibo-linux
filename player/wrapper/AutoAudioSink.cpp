#include "AutoAudioSink.hpp"

Gst::AutoAudioSink::AutoAudioSink()
{
    m_element = gst_element_factory_make("autoaudiosink", nullptr);
}

Gst::AutoAudioSink* Gst::AutoAudioSink::create()
{
    return new Gst::AutoAudioSink;
}

