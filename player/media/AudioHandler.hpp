#pragma once

#include "IAudioHandler.hpp"
#include "wrapper/GstFwd.hpp"

#include <sigc++/signal.h>

using SignalAudioFinished = sigc::signal<void()>;

class AudioHandler : public IAudioHandler
{
public:
    AudioHandler();

    void load(const FilePath& path) override;
    void play() override;
    void stop() override;
    void stopPlayback() override;
    void connect(OnAudioFinished callback) override;
    void setVolume(int volume) override;

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

    SignalAudioFinished m_audioFinished;

};
