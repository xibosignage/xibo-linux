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

class Audio : public Media
{
public:
    Audio(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped, double volume);
    ~Audio() override;

    void stop() override;
    void start() override;
    void start_timer() override;

    void set_volume(double volume);
    void play();

private:
    bool bus_message_watch(Gst::Message* message);
    void no_more_pads();
    void on_pad_added(Gst::Pad* pad);

private:
    bool m_muted;
    bool m_looped;

    guint m_watch_id;
    bool m_audio_ended = false;
    std::shared_ptr<spdlog::logger> m_logger;

    Gst::Pipeline* m_pipeline = nullptr;
    Gst::FileSrc* m_source = nullptr;
    Gst::Decodebin* m_decodebin = nullptr;
    Gst::Volume* m_volume = nullptr;
    Gst::AudioConvert* m_audio_converter = nullptr;
    Gst::AutoAudioSink* m_audio_sink = nullptr;
};
