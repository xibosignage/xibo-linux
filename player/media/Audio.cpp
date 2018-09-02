#include "Audio.hpp"
#include "wrapper/Pipeline.hpp"
#include "wrapper/AudioConvert.hpp"
#include "wrapper/Volume.hpp"
#include "wrapper/Queue.hpp"
#include "wrapper/Decodebin.hpp"
#include "wrapper/FileSrc.hpp"
#include "wrapper/AutoAudioSink.hpp"
#include "wrapper/Element.hpp"
#include "wrapper/Pad.hpp"

#include "constants.hpp"

const double MIN_GST_VOLUME = 0.0;
const double MAX_GST_VOLUME = 1.0;

namespace ph = std::placeholders;

Audio::Audio(int id, int duration, const std::string& uri, bool muted, bool looped, double volume) :
    Media(id, -1, -1, duration, Render::Native, uri), m_muted(muted), m_looped(looped) // FIXME
{
    gst_init(nullptr, nullptr);
    m_logger = spdlog::get(LOGGER);

    m_pipeline = Gst::Pipeline::create();
    m_source = Gst::FileSrc::create();
    m_decodebin = Gst::Decodebin::create();
    m_audioConverter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_audioSink = Gst::AutoAudioSink::create();

    if(!m_pipeline || !m_source || !m_decodebin || !m_audioConverter || !m_volume || !m_audioSink)
    {
        throw std::runtime_error("[Audio] One element could not be created");
    }

    m_source->setLocation(uri);
    m_pipeline->addBusWatch(sigc::mem_fun(*this, &Audio::busMessageWatch));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
    m_source->link(m_decodebin);
    m_audioConverter->link(m_audioConverter)->link(m_volume)->link(m_audioSink);

    m_decodebin->signalPadAdded().connect(sigc::mem_fun(*this, &Audio::onPadAdded));
    m_decodebin->signalNoMorePads().connect(sigc::mem_fun(*this, &Audio::noMorePads));

    setVolume(m_muted ? MIN_GST_VOLUME : volume);
}

Audio::~Audio()
{
    m_logger->debug("[Audio] Returned, stopping playback");
}

bool Audio::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            m_logger->error("{}", err.getText());
            m_logger->error("[Audio] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            m_logger->debug("[Audio] End of stream");
            m_audioEnded = true;
            if(m_looped)
                play();
            else
                mediaTimeout().emit();
            break;
        }
        default:
            break;
    }
    return true;
}

void Audio::noMorePads()
{
    m_logger->debug("[Audio] No more pads");
}

void Audio::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    m_logger->debug("[Audio] Pad added");

    auto sinkpad = m_audioConverter->getStaticPad("sink");
    pad->link(sinkpad);
}

void Audio::setVolume(double volume)
{
    m_volume->setVolume(volume);
}

void Audio::play()
{
    if(m_audioEnded)
    {
        m_audioEnded = false;
        m_pipeline->setState(Gst::State::NULL_STATE);
        m_logger->debug("[Audio] Restarting");
    }
    else
    {
        m_logger->debug("[Audio] Starting");
    }
    m_pipeline->setState(Gst::State::PLAYING);
}

void Audio::stop()
{
    Media::stop();
    m_logger->debug("[Audio] Stopped");
    m_pipeline->setState(Gst::State::NULL_STATE);
    m_audioEnded = true;
}

void Audio::start()
{
    Media::start();
    play();
}

void Audio::startTimer()
{
    if(duration())
    {
        Media::startTimer();
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
    return m_volume->getVolume();
}

