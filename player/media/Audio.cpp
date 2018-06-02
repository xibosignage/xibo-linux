#include "Audio.hpp"
#include "control/Region.hpp"

const double MIN_GST_VOLUME = 0.0;
const double MAX_GST_VOLUME = 1.0;

namespace ph = std::placeholders;

Audio::Audio(int id, int duration, const std::string& uri, bool muted, bool looped, double volume) :
    Media(id, {}, duration, Render::Native, uri), m_muted(muted), m_looped(looped)
{
    gst_init(nullptr, nullptr);
    m_logger = spdlog::get(LOGGER);

    m_pipeline = Gst::Pipeline::create();
    m_source = Gst::FileSrc::create();
    m_decodebin = Gst::Decodebin::create();
    m_audio_converter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_audio_sink = Gst::AutoAudioSink::create();

    if(!m_pipeline || !m_source || !m_decodebin || !m_audio_converter || !m_volume || !m_audio_sink)
    {
        throw std::runtime_error("[Audio] One element could not be created");
    }

    m_source->set_location(uri);
    m_pipeline->add_bus_watch(sigc::mem_fun(*this, &Audio::bus_message_watch));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_audio_converter)->add(m_volume)->add(m_audio_sink);
    m_source->link(m_decodebin);
    m_audio_converter->link(m_audio_converter)->link(m_volume)->link(m_audio_sink);

    m_decodebin->signal_pad_added().connect(sigc::mem_fun(*this, &Audio::on_pad_added));
    m_decodebin->signal_no_more_pads().connect(sigc::mem_fun(*this, &Audio::no_more_pads));

    set_volume(m_muted ? MIN_GST_VOLUME : volume);
}

Audio::~Audio()
{
    m_logger->debug("[Audio] Returned, stopping playback");
}

bool Audio::bus_message_watch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parse_error();
            m_logger->error("{}", err.get_text());
            m_logger->error("[Audio] Debug details: {}", err.get_debug_info());
            break;
        }
        case Gst::MessageType::EOS:
        {
            m_logger->debug("[Audio] End of stream");
            m_audio_ended = true;
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

void Audio::no_more_pads()
{
    m_logger->debug("[Audio] No more pads");
}

void Audio::on_pad_added(const Gst::RefPtr<Gst::Pad>& pad)
{
    m_logger->debug("[Audio] Pad added");

    auto sinkpad = m_audio_converter->get_static_pad("sink");
    pad->link(sinkpad);
}

void Audio::set_volume(double volume)
{
    m_volume->set_volume(volume);
}

void Audio::play()
{
    if(m_audio_ended) // NOTE add with looping
    {
        m_pipeline->set_state(Gst::State::NULL_STATE);
        m_audio_ended = false;
        m_pipeline->set_state(Gst::State::PLAYING);
        m_logger->debug("[Audio] Restarting");
    }
    m_pipeline->set_state(Gst::State::PLAYING);
    m_logger->debug("[Audio] Running");
}

void Audio::stop()
{
    Media::stop();
    m_logger->debug("[Audio] Stopped");
    m_pipeline->set_state(Gst::State::NULL_STATE);
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

bool Audio::muted() const
{
    return m_muted;
}

bool Audio::looped() const
{
    return m_looped;
}

double Audio::volume() const
{
    return m_volume->get_volume();
}

