#include "Video.hpp"
#include "customsink/XiboVideoSink.hpp"
#include "control/Region.hpp"

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

    m_pipeline = Gst::Pipeline::create("pipeline");
    m_source = Gst::FileSrc::create();
    m_decodebin = Gst::Decodebin::create();
    m_video_converter = Gst::VideoConvert::create();
    m_audio_converter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_video_scale = Gst::VideoScale::create();
    m_audio_sink = Gst::AutoAudioSink::create();
    m_queue = Gst::Queue::create();
    m_video_sink = Gst::Element::create("xibovideosink");

    auto sink = GST_XIBOVIDEOSINK(m_video_sink->get_handler());
    gst_xibovideosink_set_handler(sink, &m_video_window);

    if(!m_pipeline || !m_source || !m_decodebin || !m_video_converter || !m_video_scale ||
       !m_video_sink || !m_audio_converter || !m_volume || !m_audio_sink || !m_queue)
    {
        throw std::runtime_error("[Video] One element could not be created");
    }

    m_source->set_location(uri);
    m_queue->set_max_size_buffers(DEFAULT_VIDEO_BUFFER);
    m_pipeline->add_bus_watch(sigc::mem_fun(*this, &Video::bus_message_watch));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_video_converter)->add(m_queue)->add(m_video_scale)->add(m_video_sink)->add(m_audio_converter)->add(m_volume)->add(m_audio_sink);
    m_source->link(m_decodebin);
    m_video_converter->link(m_video_scale)->link(m_queue)->link(m_video_sink);
    m_audio_converter->link(m_volume)->link(m_audio_sink);

    m_decodebin->signal_pad_added().connect(sigc::mem_fun(*this, &Video::on_pad_added));
    m_decodebin->signal_no_more_pads().connect(sigc::mem_fun(*this, &Video::no_more_pads));

    m_video_window.set_size_request(region.size().width, region.size().height);
    region.request_handler().connect([=]{
        handler_requested().emit(m_video_window, DEFAULT_POINT);
    });

    set_volume(m_muted ? MIN_VOLUME : MAX_VOLUME);
}

Video::~Video()
{
    m_logger->debug("[Video] Returned, stopping playback");
}

bool Video::bus_message_watch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parse_error();
            m_logger->error("{}", err.get_text());
            m_logger->error("[Video] Debug details: {}", err.get_debug_info());
            break;
        }
        case Gst::MessageType::EOS:
        {
            m_logger->debug("[Video] End of stream");
            m_video_ended = true;
            if(m_looped)
                play();
            else
                media_timeout().emit();
            break;
        }
        default:
            break;
    }
    return true;
}

void Video::no_more_pads()
{
    auto pad = m_decodebin->get_static_pad("src_1");
    m_logger->debug("[Video] No more pads");

    if(!pad)
    {
        m_audio_converter->set_state(Gst::State::NULL_STATE);
        m_volume->set_state(Gst::State::NULL_STATE);
        m_audio_sink->set_state(Gst::State::NULL_STATE);
        m_pipeline->remove(m_audio_converter)->remove(m_volume)->remove(m_audio_sink);
    }
}

void Video::on_pad_added(const Gst::RefPtr<Gst::Pad>& pad)
{
    Gst::RefPtr<Gst::Pad> sinkpad;
    m_logger->debug("[Video] Pad added");

    // src_0 for video stream
    auto video_pad = m_decodebin->get_static_pad("src_0");
    // src1 for audio stream
    auto audio_pad = m_decodebin->get_static_pad("src_1");

    if(video_pad && !audio_pad)
    {
        m_logger->debug("[Video] Video pad");

        auto caps = video_pad->get_current_caps();
        if(caps)
        {
            auto strct = caps->get_structure(0);
            m_logger->info("[Video] width: {} height: {}", strct->get_height(), strct->get_width());
        }

        sinkpad = m_video_converter->get_static_pad("sink");
        pad->link(sinkpad);
    }
    else if(audio_pad)
    {
        m_logger->debug("[Video] Audio pad");
        sinkpad = m_audio_converter->get_static_pad("sink");
        pad->link(sinkpad);
    }
}

void Video::set_volume(double volume)
{
    m_volume->set_volume(volume);
}

// NOTE Test looping video now
void Video::play()
{
    if(m_video_ended)
    {
        if(!m_pipeline->seek(1.0, Gst::Format::TIME, Gst::SeekFlags::FLUSH,
                             Gst::SeekType::SET, 0, Gst::SeekType::END, GST_CLOCK_TIME_NONE))
        {
            m_logger->error("[Video] Error during restart");
            return;
        }
        m_video_ended = false;
    }
    m_pipeline->set_state(Gst::State::PLAYING);
    m_logger->debug("[Video] Running");
}

void Video::stop()
{
    Media::stop();
    m_video_window.hide();
    m_logger->debug("[Video] Stopped");
    m_pipeline->set_state(Gst::State::PAUSED);
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
