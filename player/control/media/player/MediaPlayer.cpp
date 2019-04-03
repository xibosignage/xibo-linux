#include "MediaPlayer.hpp"

#include "constants.hpp"

#include "wrapper/Pipeline.hpp"
#include "wrapper/VideoConvert.hpp"
#include "wrapper/AudioConvert.hpp"
#include "wrapper/Volume.hpp"
#include "wrapper/VideoScale.hpp"
#include "wrapper/Queue.hpp"
#include "wrapper/Decodebin.hpp"
#include "wrapper/FileSrc.hpp"
#include "wrapper/AutoAudioSink.hpp"
#include "wrapper/Element.hpp"
#include "wrapper/Pad.hpp"
#include "wrapper/Caps.hpp"
#include "wrapper/Capsfilter.hpp"
#include "wrapper/Inspector.hpp"
#include "customsink/XiboVideoSink.hpp"

#include "utils/logger/Logging.hpp"
#include "utils/uri/Uri.hpp"

#include <boost/format.hpp>

namespace ph = std::placeholders;

const int DEFAULT_VIDEO_BUFFER = 500;
const unsigned int INSPECTOR_TIMEOUT = 5;

MediaPlayer::MediaPlayer()
{
    if(!gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "xibovideosink", "Video Sink Plugin for gstreamer",
                                   pluginInit, "0.1", "GPL", "source", "package", "http://github.com/Stivius"))
    {
        throw std::runtime_error("XiboVideoSink was not registered");
    }

    m_pipeline = Gst::Pipeline::create("pipeline");
    m_source = Gst::FileSrc::create();
    m_decodebin = Gst::Decodebin::create();
    m_inspector = Gst::Inspector::create(INSPECTOR_TIMEOUT);

    m_videoConverter = Gst::VideoConvert::create();
    m_queue = Gst::Queue::create();
    m_videoScale = Gst::VideoScale::create();
    m_videoSink = Gst::Element::create("xibovideosink");
    m_capsfilter = Gst::Capsfilter::create();

    m_audioConverter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_audioSink = Gst::AutoAudioSink::create();

    checkGstElements();
    init();
}

MediaPlayer::~MediaPlayer()
{
    stop();
}

void MediaPlayer::setOutputWindow(const std::shared_ptr<VideoWindow>& window)
{
    m_videoWindow = window;

    m_videoWindow->shown().connect([this](){
        setVideoSize(m_videoWindow->width(), m_videoWindow->height());
    });

    auto sink = GST_XIBOVIDEOSINK(m_videoSink->getHandler());
    gst_xibovideosink_set_handler(sink, m_videoWindow.get());
}

void MediaPlayer::setVideoSize(int width, int height)
{
    boost::format videoFmt{"video/x-raw, width = (int)%1%, height = (int)%2%"};

    m_capsfilter->setCaps(Gst::Caps::create((videoFmt % width % height).str()));
}

void MediaPlayer::checkGstElements()
{
    if(!m_pipeline || !m_source || !m_decodebin || !m_videoConverter || !m_videoScale ||
       !m_videoSink || !m_queue || !m_capsfilter || !m_audioConverter || !m_volume || !m_audioSink)
    {
        throw std::runtime_error("[MediaPlayer] One element could not be created");
    }
}

void MediaPlayer::inspectFile(const Uri& uri)
{
    auto result = m_inspector->discover(uri.string());

    m_pipeline->add(m_source)->add(m_decodebin);
    m_source->link(m_decodebin);

    if(result.hasVideoStream())
    {
        m_pipeline->add(m_videoConverter)->add(m_queue)->add(m_videoScale)->add(m_videoSink)->add(m_capsfilter);
        m_videoConverter->link(m_queue)->link(m_videoScale)->link(m_capsfilter)->link(m_videoSink);
    }

    if(result.hasAudioStream())
    {
        m_pipeline->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
        m_audioConverter->link(m_volume)->link(m_audioSink);
    }
}

void MediaPlayer::init()
{
    m_queue->setMaxSizeBuffers(DEFAULT_VIDEO_BUFFER);
    m_pipeline->addBusWatch(sigc::mem_fun(*this, &MediaPlayer::busMessageWatch));

    m_decodebin->signalPadAdded().connect(sigc::mem_fun(*this, &MediaPlayer::onPadAdded));
    m_decodebin->signalNoMorePads().connect(sigc::mem_fun(*this, &MediaPlayer::noMorePads));
}

bool MediaPlayer::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            Log::error("{}", err.getText());
            Log::error("[MediaPlayer] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            Log::debug("[MediaPlayer] End of stream");
            m_pipeline->setState(Gst::State::NULL_STATE);
            m_playbackFinished.emit();
            break;
        }
        default:
            break;
    }
    return true;
}

void MediaPlayer::noMorePads()
{
    Log::debug("[MediaPlayer] No more pads");
}

void MediaPlayer::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    Gst::RefPtr<Gst::Pad> sinkpad;
    Log::debug("[MediaPlayer] Pad added");

    auto mediaType = pad->mediaType();
    if(mediaType == Gst::MediaType::Video)
    {
        Log::debug("[MediaPlayer] Video pad");

        sinkpad = m_videoConverter->getStaticPad("sink");
        pad->link(sinkpad);
    }
    else if(mediaType == Gst::MediaType::Audio)
    {
        Log::debug("[MediaPlayer] Audio pad");

        sinkpad = m_audioConverter->getStaticPad("sink");
        pad->link(sinkpad);
    }
}

void MediaPlayer::load(const Uri& uri)
{
    m_source->setLocation(uri.path());

    inspectFile(uri);
}

void MediaPlayer::play()
{
    if(m_videoWindow)
    {
        m_videoWindow->show();
    }
    m_pipeline->setState(Gst::State::PLAYING);
}

void MediaPlayer::stop()
{
    if(m_videoWindow)
    {
        m_videoWindow->hide();
    }
    m_pipeline->setState(Gst::State::NULL_STATE);
}

void MediaPlayer::setVolume(int volume)
{
    m_volume->setVolume(volume / static_cast<double>(MAX_VOLUME));
}

SignalPlaybackFinished MediaPlayer::playbackFinished()
{
    return m_playbackFinished;
}
