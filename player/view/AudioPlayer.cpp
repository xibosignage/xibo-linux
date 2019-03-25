#include "AudioPlayer.hpp"

#include "wrapper/Pipeline.hpp"
#include "wrapper/AudioConvert.hpp"
#include "wrapper/Volume.hpp"
#include "wrapper/Decodebin.hpp"
#include "wrapper/FileSrc.hpp"
#include "wrapper/AutoAudioSink.hpp"
#include "wrapper/Element.hpp"
#include "wrapper/Pad.hpp"

#include "utils/logger/Logging.hpp"
#include "utils/uri/Uri.hpp"

namespace ph = std::placeholders;

AudioPlayer::AudioPlayer()
{
    m_pipeline = Gst::Pipeline::create();
    m_source = Gst::FileSrc::create();
    m_decodebin = Gst::Decodebin::create();
    m_audioConverter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_audioSink = Gst::AutoAudioSink::create();

    checkGstElements();

    init();
}

void AudioPlayer::checkGstElements()
{
    if(!m_pipeline || !m_source || !m_decodebin || !m_audioConverter || !m_volume || !m_audioSink)
        throw std::runtime_error("[AudioHandler] One element could not be created");
}

void AudioPlayer::init()
{
    m_pipeline->addBusWatch(std::bind(&AudioPlayer::busMessageWatch, this, ph::_1));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
    m_source->link(m_decodebin);
    m_audioConverter->link(m_audioConverter)->link(m_volume)->link(m_audioSink);

    m_decodebin->signalPadAdded().connect(std::bind(&AudioPlayer::onPadAdded, this, ph::_1));
    m_decodebin->signalNoMorePads().connect(std::bind(&AudioPlayer::noMorePads, this));
}

bool AudioPlayer::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            Log::error("{}", err.getText());
            Log::error("[AudioHandler] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            Log::debug("[AudioHandler] End of stream");
            m_pipeline->setState(Gst::State::NULL_STATE);
            pushEvent(DurationExpiredEvent{});
            break;
        }
        default:
            break;
    }
    return true;
}

void AudioPlayer::noMorePads()
{
    Log::debug("[AudioHandler] No more pads");
}

void AudioPlayer::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    Log::debug("[AudioHandler] Pad added");

    auto sinkpad = m_audioConverter->getStaticPad("sink");
    pad->link(sinkpad);
}

void AudioPlayer::load(const Uri& uri)
{
    m_source->setLocation(uri.path());
}

void AudioPlayer::play()
{
    m_pipeline->setState(Gst::State::PLAYING);
}

void AudioPlayer::stop()
{
    m_pipeline->setState(Gst::State::NULL_STATE);
}

void AudioPlayer::setVolume(int volume)
{
    m_volume->setVolume(volume / static_cast<double>(MAX_VOLUME));
}
