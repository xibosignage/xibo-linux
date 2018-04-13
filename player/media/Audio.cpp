#include "Audio.hpp"
#include "control/Region.hpp"
#include "utils/BindWrapper.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;

namespace ph = std::placeholders;

Audio::Audio(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped, double volume) :
    Media(region, id, duration, Render::Native, uri), m_muted(muted), m_looped(looped)
{
    gst_init(nullptr, nullptr);
    m_logger = spdlog::get(LOGGER);

    m_pipeline = gst_pipeline_new("player");
    m_source = gst_element_factory_make("filesrc", nullptr);
    m_decodebin = gst_element_factory_make("decodebin", nullptr);
    m_audio_converter = gst_element_factory_make("audioconvert", nullptr);
    m_volume = gst_element_factory_make("volume", nullptr);
    m_audio_sink = gst_element_factory_make("autoaudiosink", nullptr);

    if(!m_pipeline || !m_source || !m_decodebin || !m_audio_converter || !m_volume || !m_audio_sink)
    {
        // FIXME exception
        m_logger->critical("One element could not be created");
    }

    g_object_set(m_source, "location", uri.c_str(), nullptr);

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
    auto bus_message_watch = get_wrapper<2, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&Audio::bus_message_watch, this, ph::_1, ph::_2, ph::_3));
    m_watch_id = gst_bus_add_watch(bus, bus_message_watch, nullptr);
    g_object_unref(bus);

    gst_bin_add_many(GST_BIN(m_pipeline), m_source, m_decodebin, m_audio_converter, m_volume, m_audio_sink, nullptr);
    gst_element_link(m_source, m_decodebin);
    gst_element_link_many(m_audio_converter, m_volume, m_audio_sink, nullptr);

    auto on_pad_added = get_wrapper<0, void, GstElement*, GstPad*, gpointer>(std::bind(&Audio::on_pad_added, this, ph::_1, ph::_2, ph::_3));
    g_signal_connect_data(m_decodebin, "pad-added", reinterpret_cast<GCallback>(on_pad_added), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto no_more_pads = get_wrapper<1, void, GstElement*, gpointer>(std::bind(&Audio::no_more_pads, this, ph::_1, ph::_2));
    g_signal_connect_data(m_decodebin, "no-more-pads", reinterpret_cast<GCallback>(no_more_pads), nullptr, nullptr, static_cast<GConnectFlags>(0));

    set_volume(m_muted ? MIN_VOLUME : volume);
}

Audio::~Audio()
{
    m_logger->debug("Returned, stopping audio playback");
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
    g_object_unref(m_pipeline);
    g_source_remove(m_watch_id);
}

gboolean Audio::bus_message_watch(GstBus*, GstMessage* message, gpointer)
{
    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ERROR:
    {
        char* debug = nullptr;
        GError* err = nullptr;

        gst_message_parse_error(message, &err, &debug);
        m_logger->error("{}", err->message);

        if(debug)
        {
            m_logger->error("Debug details: {}", debug);
            g_free(debug);
        }

        g_error_free(err);
        break;
    }
    case GST_MESSAGE_EOS:
        m_logger->debug("End of audio stream");
        m_audio_ended = true;
        if(m_looped)
            play();
        else
            media_stopped().emit();
        break;
    default:
        break;
    }

    return true;
}

void Audio::no_more_pads(GstElement*, gpointer)
{
    m_logger->debug("No more pads");
}

void Audio::on_pad_added(GstElement*, GstPad* pad, gpointer)
{
    m_logger->debug("Audio pad added");

    GstPad* sinkpad = gst_element_get_static_pad(m_audio_converter, "sink");
    gst_pad_link(pad, sinkpad);

    gst_object_unref(sinkpad);
}

void Audio::set_volume(double _volume)
{
    g_object_set(m_volume, "volume", _volume, nullptr);
}

void Audio::play()
{
    if(m_audio_ended)
    {
        if(!gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                             GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_END, GST_CLOCK_TIME_NONE))
        {
            m_logger->error("Error while restarting audio");
            return;
        }
        m_audio_ended = false;
    }
    gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
    m_logger->debug("Running audio");
}

void Audio::stop()
{
    Media::stop();
    m_logger->debug("Stopped audio");
    gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
    m_audio_ended = true;
}

void Audio::start()
{
    Media::start();
    play();
}

void Audio::start_timer()
{
    if(duration())
    {
        Media::start_timer();
    }
}

