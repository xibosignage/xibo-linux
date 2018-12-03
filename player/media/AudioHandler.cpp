#include "AudioHandler.hpp"

#include "wrapper/Pipeline.hpp"
#include "wrapper/AudioConvert.hpp"
#include "wrapper/Volume.hpp"
#include "wrapper/Decodebin.hpp"
#include "wrapper/FileSrc.hpp"
#include "wrapper/AutoAudioSink.hpp"
#include "wrapper/Element.hpp"
#include "wrapper/Pad.hpp"

#include "utils/Utilities.hpp"
#include "utils/FilePath.hpp"

namespace ph = std::placeholders;

AudioHandler::AudioHandler()
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

void AudioHandler::checkGstElements()
{
    if(!m_pipeline || !m_source || !m_decodebin || !m_audioConverter || !m_volume || !m_audioSink)
        throw std::runtime_error("[AudioHandler] One element could not be created");
}

void AudioHandler::init()
{
    m_pipeline->addBusWatch(std::bind(&AudioHandler::busMessageWatch, this, ph::_1));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
    m_source->link(m_decodebin);
    m_audioConverter->link(m_audioConverter)->link(m_volume)->link(m_audioSink);

    m_decodebin->signalPadAdded().connect(std::bind(&AudioHandler::onPadAdded, this, ph::_1));
    m_decodebin->signalNoMorePads().connect(std::bind(&AudioHandler::noMorePads, this));
}

bool AudioHandler::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            Utils::logger()->error("{}", err.getText());
            Utils::logger()->error("[AudioHandler] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            Utils::logger()->debug("[AudioHandler] End of stream");
            m_pipeline->setState(Gst::State::NULL_STATE);
            m_audioFinished.emit();
            break;
        }
        default:
            break;
    }
    return true;
}

void AudioHandler::noMorePads()
{
    Utils::logger()->debug("[AudioHandler] No more pads");
}

void AudioHandler::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    Utils::logger()->debug("[AudioHandler] Pad added");

    auto sinkpad = m_audioConverter->getStaticPad("sink");
    pad->link(sinkpad);
}

void AudioHandler::load(const FilePath& path)
{
    m_source->setLocation(path.string());
}

void AudioHandler::play()
{
    m_pipeline->setState(Gst::State::PLAYING);
}

void AudioHandler::stop()
{
    m_pipeline->setState(Gst::State::NULL_STATE);
}

void AudioHandler::stopPlayback()
{
    stop();
}

void AudioHandler::connect(OnAudioFinished callback)
{
    m_audioFinished.connect(callback);
}

void AudioHandler::setVolume(int volume)
{
    m_volume->setVolume(volume / static_cast<double>(MAX_VOLUME));
}
