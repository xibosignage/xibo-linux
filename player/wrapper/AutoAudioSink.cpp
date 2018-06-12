#include "AutoAudioSink.hpp"

Gst::AutoAudioSink::AutoAudioSink()
{
    m_element = gst_element_factory_make("autoaudiosink", nullptr);
}

Gst::RefPtr<Gst::AutoAudioSink> Gst::AutoAudioSink::create()
{
    return std::shared_ptr<Gst::AutoAudioSink>(new Gst::AutoAudioSink);
}

