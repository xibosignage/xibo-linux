#pragma once

#include "wrapper/GstFwd.hpp"
#include "events/EventPublisher.hpp"

#include <functional>

class Uri;

class AudioPlayer : public EventPublisher<>
{
public:
    AudioPlayer();

    void load(const Uri& uri);
    void play();
    void stop();
    void setVolume(int volume);

private:
    bool busMessageWatch(const Gst::RefPtr<Gst::Message>& message);
    void noMorePads();
    void onPadAdded(const Gst::RefPtr<Gst::Pad>& pad);
    void init();
    void checkGstElements();

private:
    Gst::RefPtr<Gst::Pipeline> m_pipeline;
    Gst::RefPtr<Gst::FileSrc> m_source;
    Gst::RefPtr<Gst::Decodebin> m_decodebin;
    Gst::RefPtr<Gst::Volume> m_volume;
    Gst::RefPtr<Gst::AudioConvert> m_audioConverter;
    Gst::RefPtr<Gst::AutoAudioSink> m_audioSink;

};
