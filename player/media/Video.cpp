#include "Video.hpp"
#include "XiboVideoSink.hpp"
#include "control/Region.hpp"
#include "utils/BindWrapper.hpp"

const double MIN_VOLUME = 0.0;
const double MAX_VOLUME = 1.0;
const int DEFAULT_VIDEO_BUFFER = 500;

namespace ph = std::placeholders;

Video::Video(const Region& region, int id, int duration, const std::string& uri, bool muted, bool looped) :
    Media(region, id, duration, Render::Native, uri), m_muted(muted), m_looped(looped)
{
    gst_init(nullptr, nullptr);
    m_logger = spdlog::get(LOGGER);

    gst_static_pads_init(region.size().width, region.size().height);
    if(!gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "xibovideosink", "Video Sink Plugin for gstreamer",
                                   plugin_init, "0.1", "GPL", "source", "package", "http://github.com/Stivius"))
    {
        throw std::runtime_error("XiboVideoSink was not registered");
    }

    m_pipeline = gst_pipeline_new("player");
    m_source = gst_element_factory_make("filesrc", nullptr);
    m_decodebin = gst_element_factory_make("decodebin", nullptr);
    m_video_converter = gst_element_factory_make("videoconvert", nullptr);
    m_audio_converter = gst_element_factory_make("audioconvert", nullptr);
    m_volume = gst_element_factory_make("volume", nullptr);
    m_video_scale = gst_element_factory_make("videoscale", nullptr);
    m_video_sink = gst_element_factory_make("xibovideosink", nullptr);

    auto sink = GST_XIBOVIDEOSINK(m_video_sink);
    gst_xibovideosink_set_handler(sink, &m_video_window);

    m_audio_sink = gst_element_factory_make("autoaudiosink", nullptr);
    m_queue = gst_element_factory_make("queue", nullptr);
    g_object_set(m_queue, "max-size-buffers", DEFAULT_VIDEO_BUFFER, nullptr);

    if(!m_pipeline || !m_source || !m_decodebin || !m_video_converter || !m_video_scale ||
       !m_video_sink || !m_audio_converter || !m_volume || !m_audio_sink || !m_queue)
    {
        throw std::runtime_error("[Video] One element could not be created");
    }

    g_object_set(m_source, "location", uri.c_str(), nullptr);

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
    auto bus_message_watch = get_wrapper<2, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&Video::bus_message_watch, this, ph::_1, ph::_2, ph::_3));
    m_watch_id = gst_bus_add_watch(bus, bus_message_watch, nullptr);
    g_object_unref(bus);

    gst_bin_add_many(GST_BIN(m_pipeline), m_source, m_decodebin, m_video_converter, m_queue, m_video_scale, m_video_sink, m_audio_converter, m_volume, m_audio_sink, nullptr);
    gst_element_link(m_source, m_decodebin);
    gst_element_link_many(m_video_converter, m_video_scale, m_queue, m_video_sink, nullptr);
    gst_element_link_many(m_audio_converter, m_volume, m_audio_sink, nullptr);

    auto on_pad_added = get_wrapper<0, void, GstElement*, GstPad*, gpointer>(std::bind(&Video::on_pad_added, this, ph::_1, ph::_2, ph::_3));
    g_signal_connect_data(m_decodebin, "pad-added", reinterpret_cast<GCallback>(on_pad_added), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto no_more_pads = get_wrapper<1, void, GstElement*, gpointer>(std::bind(&Video::no_more_pads, this, ph::_1, ph::_2));
    g_signal_connect_data(m_decodebin, "no-more-pads", reinterpret_cast<GCallback>(no_more_pads), nullptr, nullptr, static_cast<GConnectFlags>(0));

    m_video_window.set_size_request(region.size().width, region.size().height);
    region.request_handler().connect([=]{
        handler_requested().emit(m_video_window, DEFAULT_POINT);
    });

    set_volume(m_muted ? MIN_VOLUME : MAX_VOLUME);
}

Video::~Video()
{
    m_logger->debug("[Video] Returned, stopping playback");
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
    g_object_unref(m_pipeline);
    g_source_remove(m_watch_id);
}

gboolean Video::bus_message_watch(GstBus*, GstMessage* message, gpointer)
{
    switch (GST_MESSAGE_TYPE(message))
    {
        case GST_MESSAGE_ERROR:
        {
            gchar *debug = nullptr;
            GError *err = nullptr;

            gst_message_parse_error(message, &err, &debug);
            m_logger->error("{}", err->message);

            if(debug)
            {
                m_logger->error("[Video] Debug details: {}", debug);
                g_free(debug);
            }

            g_error_free(err);
            break;
        }
        case GST_MESSAGE_EOS:
            m_logger->debug("[Video] End of stream");
            m_video_ended = true;
            if(m_looped)
                play();
            else
                media_timeout().emit();
            break;
        default:
            break;
    }
    return true;
}

void Video::no_more_pads(GstElement*, gpointer)
{
    auto pad = gst_element_get_static_pad(m_decodebin, "src_1");
    m_logger->debug("[Video] No more pads");

    if(!pad)
    {
        gst_element_set_state(m_audio_converter, GST_STATE_NULL);
        gst_element_set_state(m_volume, GST_STATE_NULL);
        gst_element_set_state(m_audio_sink, GST_STATE_NULL);
        gst_bin_remove_many(GST_BIN(m_pipeline), m_audio_converter, m_volume, m_audio_sink, nullptr);
    }
    else
    {
        gst_object_unref(pad);
    }
}

void Video::on_pad_added(GstElement*, GstPad* pad, gpointer)
{
    GstPad* sinkpad;
    m_logger->debug("[Video] Pad added");

    // src_0 for video stream
    auto video_pad = gst_element_get_static_pad(m_decodebin, "src_0");
    // src1 for audio stream
    auto audio_pad = gst_element_get_static_pad(m_decodebin, "src_1");

    if(video_pad && !audio_pad)
    {
        m_logger->debug("[Video] Video pad");

        auto caps = gst_pad_get_current_caps(video_pad);
        if(caps)
        {
            auto strct = gst_caps_get_structure(caps, 0);
            int width, height;
            gst_structure_get_int(strct, "width", &width);
            gst_structure_get_int(strct, "height", &height);

            m_logger->info("[Video] width: {} height: {}", width, height);

            gst_caps_unref(caps);
        }
        sinkpad = gst_element_get_static_pad(m_video_converter, "sink");
        gst_pad_link(pad, sinkpad);

        gst_object_unref(sinkpad);
        gst_object_unref(video_pad);
    }
    else if(audio_pad)
    {
        m_logger->debug("[Video] Audio pad");

        sinkpad = gst_element_get_static_pad(m_audio_converter, "sink");
        gst_pad_link(pad, sinkpad);

        gst_object_unref(sinkpad);
        gst_object_unref(audio_pad);
    }
}

void Video::set_volume(double _volume)
{
    g_object_set(m_volume, "volume", _volume, nullptr);
}

// NOTE Test looping video now
void Video::play()
{
    if(m_video_ended)
    {
        if(!gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                             GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_END, GST_CLOCK_TIME_NONE))
        {
            m_logger->error("[Video] Error during restart");
            return;
        }
        m_video_ended = false;
    }
    gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
    m_logger->debug("[Video] Running");
}

void Video::stop()
{
    Media::stop();
    m_video_window.hide();
    m_logger->debug("[Video] Stopped");
    gst_element_set_state(m_pipeline, GST_STATE_PAUSED);
    m_video_ended = true;
}

void Video::start()
{
    Media::start();
    m_video_window.show();
    play();
}

void Video::start_timer()
{
    if(duration())
    {
        Media::start_timer();
    }
}
