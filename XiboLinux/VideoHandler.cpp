#include "VideoHandler.hpp"

#include <gdk/gdkx.h>

#include <gstreamermm/elementfactory.h>
#include <gstreamermm/videooverlay.h>
#include <gstreamermm/init.h>

VideoHandler::VideoHandler(const std::string& uri, const Size& size) :
    m_size(size)
{
    Gst::init();

    m_logger = spdlog::get(LOGGER);

    m_pipeline = Gst::Pipeline::create("player");
    m_source = Gst::ElementFactory::create_element("filesrc");
    m_decodebin = Gst::ElementFactory::create_element("decodebin");

    m_video_converter = Gst::ElementFactory::create_element("videoconvert");
    m_video_sink = Gst::ElementFactory::create_element("autovideosink");

    m_audio_converter = Gst::ElementFactory::create_element("audioconvert");
    m_volume = Gst::ElementFactory::create_element("volume");
    m_audio_sink = Gst::ElementFactory::create_element("autoaudiosink");

    if(!m_pipeline || !m_source || !m_decodebin || !m_video_converter ||
       !m_video_sink || !m_audio_converter || !m_volume || !m_audio_sink)
    {
        m_logger->critical("One element could not be created");
    }

    m_source->set_property("location", uri);

    auto bus = m_pipeline->get_bus();
    m_watch_id = bus->add_watch(sigc::mem_fun(this, &VideoHandler::bus_message_watch));

    bus->set_sync_handler(sigc::mem_fun(this, &VideoHandler::bus_sync_handler));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_video_converter)->add(m_video_sink)->add(m_audio_converter)->add(m_volume)->add(m_audio_sink);

    m_source->link(m_decodebin);
    m_video_converter->link(m_video_sink);
    m_audio_converter->link(m_volume)->link(m_audio_sink);

    m_decodebin->signal_pad_added().connect(sigc::mem_fun(this, &VideoHandler::on_pad_added));
    m_decodebin->signal_no_more_pads().connect(sigc::mem_fun(this, &VideoHandler::no_more_pads));

    create_ui();
}

VideoHandler::~VideoHandler()
{
    m_logger->debug("Returned, stopping playback");
    m_pipeline->get_bus()->remove_watch(m_watch_id);
    m_pipeline->set_state(Gst::State::STATE_NULL);
}

void VideoHandler::realize_cb()
{
    auto window = m_video_window.get_window();

    if(!window->ensure_native())
    {
        m_logger->critical("Couldn't create native window needed for GstVideoOverlay!");
    }

    m_window_handle = GDK_WINDOW_XID(window->gobj());
}

Gst::BusSyncReply VideoHandler::bus_sync_handler(const Glib::RefPtr<Gst::Bus>&, const Glib::RefPtr<Gst::Message>& message)
{
    if(!gst_is_video_overlay_prepare_window_handle_message(message->gobj()))
        return Gst::BusSyncReply::BUS_PASS;

    if(m_window_handle != 0)
    {
        auto videooverlay = Glib::RefPtr<Gst::VideoOverlay>::cast_dynamic(message->get_source());
        if(videooverlay)
        {
            videooverlay->set_window_handle(m_window_handle);
        }
    }
    else
    {
        m_logger->error("Should have obtained video_window_handle by now!");
    }

    return Gst::BusSyncReply::BUS_DROP;
}

bool VideoHandler::bus_message_watch(const Glib::RefPtr<Gst::Bus>&, const Glib::RefPtr<Gst::Message>& message)
{
    switch (message->get_message_type())
    {
    case Gst::MESSAGE_ERROR:
    {
        auto error_msg = Glib::RefPtr<Gst::MessageError>::cast_static(message);
        m_logger->error("{}", static_cast<std::string>(error_msg->parse_error().what()));
        break;
    }
    case Gst::MESSAGE_EOS:
        m_logger->debug("End of stream");
        m_video_ended = true;
        m_signal_video_ended.emit();
        break;
    default:
        break;
    }

    return true;
}

void VideoHandler::no_more_pads()
{
    auto pad = m_decodebin->get_static_pad("src_1");
    m_logger->debug("No more pads");
    if(!pad)
    {
        m_audio_converter->set_state(Gst::State::STATE_NULL);
        m_volume->set_state(Gst::State::STATE_NULL);
        m_audio_sink->set_state(Gst::State::STATE_NULL);
        m_pipeline->remove(m_audio_converter)->remove(m_volume)->remove(m_audio_sink);
    }
}

void VideoHandler::on_pad_added(const Glib::RefPtr<Gst::Pad>& pad)
{
    Glib::RefPtr<Gst::Pad> sinkpad;

    // src_0 for video stream
    auto video_pad = m_decodebin->get_static_pad("src_0");
    // src1 for audio stream
    auto audio_pad = m_decodebin->get_static_pad("src_1");

    if(video_pad && !audio_pad)
    {
        auto caps = video_pad->get_current_caps();
        if(caps)
        {
            auto strct = caps->get_structure(0);
            strct.get_field("height", m_best_size.height);
            strct.get_field("width", m_best_size.width);

            update_video_size();
            m_logger->info("height: {}, width: {}", m_best_size.height, m_best_size.width);
        }
        sinkpad = m_video_converter->get_static_pad("sink");
        pad->link(sinkpad);
    }
    else if(audio_pad)
    {
        sinkpad = m_audio_converter->get_static_pad("sink");
        pad->link(sinkpad);
    }
}

void VideoHandler::create_ui()
{
    m_video_window.set_size_request(m_size.width, m_size.height);
    m_video_window.signal_realize().connect(sigc::mem_fun(this, &VideoHandler::realize_cb));

    add(m_video_window);
}

void VideoHandler::set_volume(double _volume)
{
    m_volume->set_property("volume", _volume);
}

void VideoHandler::set_size(int width, int height)
{
    m_video_window.set_size_request(width, height);
}

void VideoHandler::play()
{
    if(m_video_ended)
    {
        if(!m_pipeline->seek(1.0, Gst::FORMAT_TIME,
                             Gst::SEEK_FLAG_FLUSH, Gst::SEEK_TYPE_SET, 0,
                             Gst::SEEK_TYPE_END, GST_CLOCK_TIME_NONE))
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
        m_pipeline->set_state(Gst::State::STATE_PLAYING);
        m_logger->debug("Running");
    }
}

sigc::signal<void>& VideoHandler::signal_video_ended()
{
    return m_signal_video_ended;
}

void VideoHandler::update_video_size()
{
    auto neededFactor = (m_best_size.width / static_cast<float>(m_best_size.height));
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
