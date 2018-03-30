#include "VideoHandler.hpp"
#include "XiboVideoSink.hpp"
#include "BindWrapper.hpp"

namespace ph = std::placeholders;

VideoHandler::VideoHandler(const std::string& file_path, const Size& size) :
    m_size(size)
{
    gst_init(nullptr, nullptr);
    m_logger = spdlog::get(LOGGER);

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
    m_video_sink = gst_element_factory_make("xibovideosink", nullptr);

    auto sink = GST_XIBOVIDEOSINK(m_video_sink);
    gst_xibovideosink_set_handler(sink, &m_video_window);

    m_audio_sink = gst_element_factory_make("autoaudiosink", nullptr);
    m_queue = gst_element_factory_make("queue", nullptr);

    if(!m_pipeline || !m_source || !m_decodebin || !m_video_converter ||
       !m_video_sink || !m_audio_converter || !m_volume || !m_audio_sink || !m_queue)
    {
        // exception
        m_logger->critical("One element could not be created");
    }

    g_object_set(m_source, "location", file_path.c_str(), nullptr);

    GstBus* bus = gst_pipeline_get_bus(GST_PIPELINE(m_pipeline));
    auto bus_message_watch = get_wrapper<2, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&VideoHandler::bus_message_watch, this, ph::_1, ph::_2, ph::_3));
    m_watch_id = gst_bus_add_watch(bus, bus_message_watch, nullptr);
    g_object_unref(bus);

    gst_bin_add_many(GST_BIN(m_pipeline), m_source, m_decodebin, m_video_converter, m_queue, m_video_sink, m_audio_converter, m_volume, m_audio_sink, nullptr);
    gst_element_link(m_source, m_decodebin);
    gst_element_link_many(m_video_converter, m_queue, m_video_sink, nullptr);
    gst_element_link_many(m_audio_converter, m_volume, m_audio_sink, nullptr);

    auto on_pad_added = get_wrapper<0, void, GstElement*, GstPad*, gpointer>(std::bind(&VideoHandler::on_pad_added, this, ph::_1, ph::_2, ph::_3));
    g_signal_connect_data(m_decodebin, "pad-added", reinterpret_cast<GCallback>(on_pad_added), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto no_more_pads = get_wrapper<1, void, GstElement*, gpointer>(std::bind(&VideoHandler::no_more_pads, this, ph::_1, ph::_2));
    g_signal_connect_data(m_decodebin, "no-more-pads", reinterpret_cast<GCallback>(no_more_pads), nullptr, nullptr, static_cast<GConnectFlags>(0));

    create_ui();
}

VideoHandler::~VideoHandler()
{
    m_logger->debug("Returned, stopping playback");
    gst_element_set_state(m_pipeline, GST_STATE_NULL);
    g_object_unref(m_pipeline);
    g_source_remove(m_watch_id);
}

gboolean VideoHandler::bus_message_watch(GstBus* bus, GstMessage* message, gpointer user_data)
{
    switch (GST_MESSAGE_TYPE(message))
    {
    case GST_MESSAGE_ERROR:
    {
        gchar *debug = nullptr;
        GError *err = nullptr;

        gst_message_parse_error(message, &err, &debug);
        m_logger->error("{}", err->message);

        if (debug) {
            m_logger->error("Debug details: {}", debug);
            g_free (debug);
        }

        g_error_free (err);
        break;
    }
    case GST_MESSAGE_EOS:
        m_logger->debug("End of stream");
        m_video_ended = true;
        m_signal_video_ended.emit();
        break;
    default:
        break;
    }

    return true;
}

void VideoHandler::no_more_pads(GstElement* decodebin, gpointer user_data)
{
    auto pad = gst_element_get_static_pad(m_decodebin, "src_1");
    m_logger->debug("No more pads");

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

void VideoHandler::on_pad_added(GstElement* decodebin, GstPad* pad, gpointer user_data)
{
    GstPad* sinkpad;
    m_logger->debug("Pad added");

    // src_0 for video stream
    auto video_pad = gst_element_get_static_pad(decodebin, "src_0");
    // src1 for audio stream
    auto audio_pad = gst_element_get_static_pad(decodebin, "src_1");

    if(video_pad && !audio_pad)
    {
        m_logger->debug("video");

        auto caps = gst_pad_get_current_caps(video_pad);
        //m_logger->debug("{}", (std::string)caps->to_string());
        if(caps)
        {
            auto strct = gst_caps_get_structure(caps, 0);
            gst_structure_get_int(strct, "height", &m_best_size.height);
            gst_structure_get_int(strct, "width", &m_best_size.width);

            m_logger->info("height: {}, width: {}", m_best_size.height, m_best_size.width);

            gst_caps_unref(caps);
        }
        sinkpad = gst_element_get_static_pad(m_video_converter, "sink");
        //m_logger->debug("{}", (std::string)sinkpad->get_pad_template()->get_caps()->to_string());
        //m_logger->debug("{}", caps->can_intersect(sinkpad->get_pad_template()->get_caps()));
        gst_pad_link(pad, sinkpad);

        gst_object_unref(sinkpad);
        gst_object_unref(video_pad);
    }
    else if(audio_pad)
    {
        m_logger->debug("audio");

        sinkpad = gst_element_get_static_pad(m_audio_converter, "sink");
        gst_pad_link(pad, sinkpad);

        gst_object_unref(sinkpad);
        gst_object_unref(audio_pad);
    }
}

void VideoHandler::create_ui()
{
    m_video_window.set_size_request(m_size.width, m_size.height);
    add(m_video_window);
}

void VideoHandler::set_volume(double _volume)
{
    g_object_set(m_volume, "volume", _volume, nullptr);
}

void VideoHandler::play()
{
    if(m_video_ended)
    {
        if(!gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME, GST_SEEK_FLAG_FLUSH,
                             GST_SEEK_TYPE_SET, 0, GST_SEEK_TYPE_END, GST_CLOCK_TIME_NONE))
        {
            m_logger->error("Error while restarting video");
        }
        else
        {
            m_video_ended = false;
        }
    }
    else
    {
        gst_element_set_state(m_pipeline, GST_STATE_PLAYING);
        m_logger->debug("Running");
    }
}

sigc::signal<void>& VideoHandler::signal_video_ended()
{
    return m_signal_video_ended;
}
