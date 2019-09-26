#include "MediaPlayer.hpp"

#include "constants.hpp"

#include "gstwrapper/AudioConvert.hpp"
#include "gstwrapper/AutoAudioSink.hpp"
#include "gstwrapper/Caps.hpp"
#include "gstwrapper/Capsfilter.hpp"
#include "gstwrapper/Decodebin.hpp"
#include "gstwrapper/Element.hpp"
#include "gstwrapper/Pad.hpp"
#include "gstwrapper/Pipeline.hpp"
#include "gstwrapper/Queue.hpp"
#include "gstwrapper/UriSrc.hpp"
#include "gstwrapper/VideoConvert.hpp"
#include "gstwrapper/VideoScale.hpp"
#include "gstwrapper/Volume.hpp"
#include "video/XiboVideoSink.hpp"

#include "common/logger/Logging.hpp"
#include "common/uri/Uri.hpp"

#include <boost/format.hpp>

namespace ph = std::placeholders;

const int InspectorTimeout = 30;

MediaPlayer::MediaPlayer()
{
    createGstElements();
    init();
}

MediaPlayer::~MediaPlayer()
{
    stopAndRemove();
}

void MediaPlayer::createGstElements()
{
    m_pipeline = Gst::Pipeline::create();
    m_decodebin = Gst::Decodebin::create();

    m_videoConverter = Gst::VideoConvert::create();
    m_queue = Gst::Queue::create();
    m_videoScale = Gst::VideoScale::create();
    m_videoSink = Gst::Element::create("xibovideosink");
    m_capsfilter = Gst::Capsfilter::create();
    m_inspector = Gst::Inspector::create(InspectorTimeout);

    m_audioConverter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_audioSink = Gst::AutoAudioSink::create();

    if (!m_pipeline || !m_decodebin || !m_videoConverter || !m_videoScale || !m_videoSink || !m_queue ||
        !m_capsfilter || !m_audioConverter || !m_volume || !m_audioSink)
    {
        throw std::runtime_error("[MediaPlayer] Error during creation");
    }
}

void MediaPlayer::init()
{
    m_pipeline->addBusWatch(std::bind(&MediaPlayer::busMessageWatch, this, ph::_1));

    m_decodebin->signalPadAdded().connect(std::bind(&MediaPlayer::onPadAdded, this, ph::_1));
    m_decodebin->signalNoMorePads().connect(std::bind(&MediaPlayer::noMorePads, this));
}

void MediaPlayer::setOutputWindow(const std::shared_ptr<IVideoWindow>& window)
{
    m_outputWindow = window;

    m_outputWindow->shown().connect([this]() {
        boost::format videoFmt{"video/x-raw, width = (int)%1%, height = (int)%2%"};
        m_capsfilter->setCaps(Gst::Caps::create((videoFmt % m_outputWindow->width() % m_outputWindow->height()).str()));
    });

    auto sink = GST_XIBOVIDEOSINK(m_videoSink->handler());
    gst_xibovideosink_set_handler(sink, m_outputWindow);
}

std::shared_ptr<IVideoWindow> MediaPlayer::outputWindow() const
{
    return m_outputWindow;
}

void MediaPlayer::load(const Uri& uri)
{
    m_source = Gst::UriSrc::create(uri.scheme());
    m_source->setLocation(uri);

    inspectFile(uri);
}

void MediaPlayer::inspectFile(const Uri& uri)
{
    m_mediaInfo = m_inspector->discover(uri.string());

    m_pipeline->add(m_source)->add(m_decodebin);
    m_source->link(m_decodebin);

    if (m_mediaInfo.videoStream)
    {
        m_pipeline->add(m_videoConverter)->add(m_queue)->add(m_videoScale)->add(m_videoSink)->add(m_capsfilter);
        m_videoConverter->link(m_queue)->link(m_videoScale)->link(m_capsfilter)->link(m_videoSink);
    }
    if (m_mediaInfo.audioStream)
    {
        m_pipeline->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
        m_audioConverter->link(m_volume)->link(m_audioSink);
    }
}

void MediaPlayer::play()
{
    if (m_outputWindow)
    {
        m_outputWindow->show();
    }
    m_pipeline->setState(Gst::State::PLAYING);
}

void MediaPlayer::stopAndRemove()
{
    if (m_outputWindow)
    {
        m_outputWindow->hide();
    }
    m_pipeline->setState(Gst::State::NULL_STATE);
}

void MediaPlayer::stopPlayback()
{
    m_pipeline->setState(Gst::State::NULL_STATE);
}

void MediaPlayer::setVolume(int volume)
{
    m_volume->setVolume(volume / static_cast<double>(MaxVolume));
}

SignalPlaybackFinished& MediaPlayer::playbackFinished()
{
    return m_playbackFinished;
}

Gst::InspectorResult MediaPlayer::mediaInfo() const
{
    return m_mediaInfo;
}

bool MediaPlayer::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch (message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            Log::error("[MediaPlayer] {}", err.getText());
            Log::debug("[MediaPlayer] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            Log::debug("[MediaPlayer] End of stream");
            m_pipeline->setState(Gst::State::NULL_STATE);
            m_playbackFinished();
            break;
        }
        default: break;
    }
    return true;
}

void MediaPlayer::noMorePads()
{
    Log::trace("[MediaPlayer] No more pads");
}

void MediaPlayer::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    auto mediaType = pad->mediaType();
    if (mediaType == Gst::MediaType::Video)
    {
        Log::trace("[MediaPlayer] Video pad added");

        auto sinkpad = m_videoConverter->staticPad("sink");
        pad->link(sinkpad);
    }
    else if (mediaType == Gst::MediaType::Audio)
    {
        Log::trace("[MediaPlayer] Audio pad added");

        auto sinkpad = m_audioConverter->staticPad("sink");
        pad->link(sinkpad);
    }
}
