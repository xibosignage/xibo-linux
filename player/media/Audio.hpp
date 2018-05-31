#include "Media.hpp"

#include <gst/gst.h>
#include <spdlog/spdlog.h>

#include "wrapper/Pipeline.hpp"
#include "wrapper/AudioConvert.hpp"
#include "wrapper/Volume.hpp"
#include "wrapper/Queue.hpp"
#include "wrapper/Decodebin.hpp"
#include "wrapper/FileSrc.hpp"
#include "wrapper/AutoAudioSink.hpp"
#include "wrapper/Element.hpp"
#include "wrapper/Pad.hpp"

class Audio : public Media
{
public:
    Audio(int id, int duration, const std::string& uri, bool muted, bool looped, double volume);
    ~Audio() override;

    void stop() override;
    void start() override;
    void start_timer() override;

    void set_volume(double volume);
    void play();

private:
    bool bus_message_watch(const Gst::RefPtr<Gst::Message>& message);
    void no_more_pads();
    void on_pad_added(const Gst::RefPtr<Gst::Pad>& pad);

private:
    bool m_muted;
    bool m_looped;

    guint m_watch_id;
    bool m_audio_ended = false;
    std::shared_ptr<spdlog::logger> m_logger;

    Gst::RefPtr<Gst::Pipeline> m_pipeline;
    Gst::RefPtr<Gst::FileSrc> m_source;
    Gst::RefPtr<Gst::Decodebin> m_decodebin;
    Gst::RefPtr<Gst::Volume> m_volume;
    Gst::RefPtr<Gst::AudioConvert> m_audio_converter;
    Gst::RefPtr<Gst::AutoAudioSink> m_audio_sink;
};
