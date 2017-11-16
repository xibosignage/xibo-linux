#include "VideoHandler.hpp"
#include "BindWrapper.hpp"

#include <gst/video/videooverlay.h>
#include <gdk/gdkx.h>

VideoHandler::VideoHandler(const std::string& , const Size& size) :
    m_size(size)
{
    gst_init(nullptr, nullptr);

    m_pipeline = gst_pipeline_new("player");
    m_source = gst_element_factory_make("filesrc", "file-source");
    m_decodebin = gst_element_factory_make("decodebin", "decoder-demuxer");

    m_videoConverter = gst_element_factory_make("videoconvert", "video-converter");
    m_videoSink = gst_element_factory_make("autovideosink", "video-output");

    m_audioConverter = gst_element_factory_make("audioconvert", "audio-converter");
    m_volume = gst_element_factory_make("volume", "volume");
    m_audioSink = gst_element_factory_make("autoaudiosink", "audio-output");

    if(!m_pipeline || !m_source || !m_decodebin || !m_videoConverter ||
       !m_videoSink || !m_audioConverter || !m_volume || !m_audioSink)
    {
        throw std::logic_error("One element could not be created");
    }

    g_object_set(reinterpret_cast<GObject*>(m_source), "location", "/home/stivius/video.webm", nullptr);

    using namespace std::placeholders;

    auto bus = gst_pipeline_get_bus(reinterpret_cast<GstPipeline*>(m_pipeline));
    gst_bus_add_signal_watch(bus);

    auto cb_message_error = get_wrapper<0, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&VideoHandler::cb_message_error, this, _1, _2, _3));
    g_signal_connect_data(bus, "message::error", reinterpret_cast<GCallback>(cb_message_error), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto cb_message_eos = get_wrapper<1, gboolean, GstBus*, GstMessage*, gpointer>(std::bind(&VideoHandler::cb_message_eos, this, _1, _2, _3));
    g_signal_connect_data(bus, "message::eos", reinterpret_cast<GCallback>(cb_message_eos), nullptr, nullptr, static_cast<GConnectFlags>(0));

    auto bus_sync_handler = get_wrapper<2, GstBusSyncReply, GstBus*, GstMessage*, gpointer>(std::bind(&VideoHandler::bus_sync_handler, this, _1, _2, _3));
    gst_bus_set_sync_handler(bus, static_cast<GstBusSyncHandler>(bus_sync_handler), nullptr, nullptr);

    gst_object_unref(bus);

    gst_bin_add_many(reinterpret_cast<GstBin*>(m_pipeline), m_source, m_decodebin, m_videoConverter, m_videoSink, m_audioConverter, m_volume, m_audioSink, nullptr);

    gst_element_link(m_source, m_decodebin);
    gst_element_link(m_videoConverter, m_videoSink);
    gst_element_link_many(m_audioConverter, m_volume, m_audioSink, nullptr);

    m_converter.video = m_videoConverter;
    m_converter.audio = m_audioConverter;

    auto on_pad_added = get_wrapper<3, void, GstElement*,GstPad*,gpointer>(std::bind(&VideoHandler::on_pad_added, this, _1, _2, _3));
    g_signal_connect_data(m_decodebin, "pad-added", reinterpret_cast<GCallback>(on_pad_added), &m_converter, nullptr, static_cast<GConnectFlags>(0));

    auto no_more_pads = get_wrapper<4, void, GstElement*, gpointer>(std::bind(&VideoHandler::no_more_pads, this, _1, _2));
    g_signal_connect_data(m_decodebin, "no-more-pads", reinterpret_cast<GCallback>(no_more_pads), nullptr, nullptr, static_cast<GConnectFlags>(0));

    printf("Now playing: %s\n", "video2.mp4");
    gst_element_set_state (m_pipeline, GST_STATE_PLAYING);

    printf("Running...\n");

    create_ui();
}

VideoHandler::~VideoHandler()
{
    printf("Returned, stopping playback\n");
    gst_element_set_state (m_pipeline, GST_STATE_NULL);

    printf("Deleting pipeline\n");
    gst_object_unref(reinterpret_cast<GstObject*>(m_pipeline));
}

void VideoHandler::realize_cb()
{
    auto window = m_videoWindow.get_window();

    if(!window->ensure_native())
    {
        throw std::logic_error("Couldn't create native window needed for GstVideoOverlay!");
    }

    m_windowHandle = GDK_WINDOW_XID(window->gobj());
}

GstBusSyncReply VideoHandler::bus_sync_handler(GstBus*, GstMessage* message, gpointer)
{
    if(!gst_is_video_overlay_prepare_window_handle_message(message))
        return GST_BUS_PASS;

    if(m_windowHandle != 0)
    {
        auto sink = reinterpret_cast<GstMessage*>(message)->src;
        auto overlay = reinterpret_cast<GstVideoOverlay*>(sink);
        gst_video_overlay_set_window_handle (overlay, m_windowHandle);
    }
    else
    {
        printf("Should have obtained video_window_handle by now!");
    }

    gst_message_unref(message);
    return GST_BUS_DROP;
}

gboolean VideoHandler::cb_message_error(GstBus*, GstMessage* msg, gpointer )
{
    gchar* debug;
    GError* error;

    gst_message_parse_error(msg, &error, &debug);
    g_free(debug);

    printf("Error: %s\n", error->message);
    g_error_free(error);

    return true;
}

gboolean VideoHandler::cb_message_eos(GstBus*, GstMessage*, gpointer)
{
    printf("End of stream\n");
    if(!gst_element_seek(m_pipeline, 1.0, GST_FORMAT_TIME,
                         GST_SEEK_FLAG_FLUSH, GST_SEEK_TYPE_SET, 0,
                         GST_SEEK_TYPE_END, GST_CLOCK_TIME_NONE))
    {
        printf("failure\n");
        gtk_main_quit();
    }
    else
    {
        printf("success\n");
    }
    return true;
}

void VideoHandler::no_more_pads(GstElement* element, gpointer)
{
    auto pad = gst_element_get_static_pad(element, "src_1");
    printf("No more pads\n");
    if(pad)
    {
        gst_object_unref(pad);
    }
    else
    {
        gst_element_set_state(m_audioConverter, GST_STATE_NULL);
        gst_element_set_state(m_volume, GST_STATE_NULL);
        gst_element_set_state(m_audioSink, GST_STATE_NULL);
        gst_bin_remove_many(reinterpret_cast<GstBin*>(m_pipeline), m_audioConverter, m_volume, m_audioSink, nullptr);
    }
}

void VideoHandler::on_pad_added(GstElement* element, GstPad* pad, gpointer data)
{
    GstPad* sinkpad, *video_pad, *audio_pad;
    GstCaps* caps;
    GstStructure* str;
    Converter* converter = reinterpret_cast<Converter*>(data);

    // src_0 for video stream
    video_pad = gst_element_get_static_pad(element, "src_0");
    if(video_pad)
    {
        caps = gst_pad_get_current_caps(video_pad);
        if(caps)
        {
            str = gst_caps_get_structure(caps, 0);

            gst_structure_get_int(str, "height", &m_bestSize.height);
            gst_structure_get_int(str, "width", &m_bestSize.width);

            update_video_size();

            printf("height: %d, width: %d\n", m_bestSize.height, m_bestSize.width);
            gst_caps_unref(caps);
        }
        sinkpad = gst_element_get_static_pad(converter->video, "sink");
        gst_pad_link(pad, sinkpad);
        gst_object_unref(sinkpad);

        gst_object_unref(video_pad);
    }

    // src1 for audio stream
    audio_pad = gst_element_get_static_pad(element, "src_1");
    if(audio_pad)
    {
        sinkpad = gst_element_get_static_pad(converter->audio, "sink");
        gst_pad_link(pad, sinkpad);
        gst_object_unref(sinkpad);

        gst_object_unref(audio_pad);
    }
}

void VideoHandler::create_ui()
{
    m_videoWindow.set_size_request(m_size.width, m_size.height);
    m_videoWindow.signal_realize().connect(sigc::mem_fun(this, &VideoHandler::realize_cb));

    add(m_videoWindow);
}

void VideoHandler::set_volume(double _volume)
{
    g_object_set(reinterpret_cast<GObject*>(m_volume), "volume", _volume, nullptr);
}

void VideoHandler::set_size(int width, int height)
{
    m_videoWindow.set_size_request(width, height);
}

void VideoHandler::update_video_size()
{
    auto neededFactor = (m_bestSize.width / static_cast<float>(m_bestSize.height));
    auto currentFactor = (m_size.width / static_cast<float>(m_size.height));

    if(currentFactor > neededFactor)
    {
        set_size(std::round(m_size.height * neededFactor), m_size.height);
    }
    else
    {
        set_size(m_size.width, std::round(m_size.width / neededFactor));
    }
}
