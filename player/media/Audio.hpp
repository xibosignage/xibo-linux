#include "Media.hpp"

#include <gst/gst.h>
#include <spdlog/spdlog.h>

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
    gboolean bus_message_watch(GstBus* bus, GstMessage* message, gpointer user_data);
    void no_more_pads(GstElement* decodebin, gpointer user_data);
    void on_pad_added(GstElement* decodebin, GstPad* pad, gpointer user_data);

private:
    bool m_muted;
    bool m_looped;

    guint m_watch_id;
    bool m_audio_ended = false;
    std::shared_ptr<spdlog::logger> m_logger;

    GstElement* m_pipeline;
    GstElement* m_source;
    GstElement* m_decodebin;
    GstElement* m_volume;
    GstElement* m_audio_converter;
    GstElement* m_audio_sink;
};
