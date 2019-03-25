#include "VideoPlayer.hpp"

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

#include "utils/Logger.hpp"
#include "utils/Uri.hpp"

#include <boost/format.hpp>

namespace ph = std::placeholders;

const int DEFAULT_VIDEO_BUFFER = 500;
const unsigned int INSPECTOR_TIMEOUT = 5;

VideoPlayer::VideoPlayer(DrawingArea& window) :
    m_videoWindow(window)
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

    m_videoWindow.subscribe(EventType::WidgetShown, [this](const Event&){
        setVideoSize(m_videoWindow.width(), m_videoWindow.height());
    });

    checkGstElements();
    init();
}

VideoPlayer::~VideoPlayer()
{
    stop();
}

void VideoPlayer::setVideoSize(int width, int height)
{
    boost::format videoFmt{"video/x-raw, width = (int)%1%, height = (int)%2%"};

    m_capsfilter->setCaps(Gst::Caps::create((videoFmt % width % height).str()));
}

void VideoPlayer::checkGstElements()
{
    if(!m_pipeline || !m_source || !m_decodebin || !m_videoConverter || !m_videoScale ||
       !m_videoSink || !m_queue || !m_capsfilter || !m_audioConverter || !m_volume || !m_audioSink)
    {
        throw std::runtime_error("[VideoHandler] One element could not be created");
    }
}

void VideoPlayer::inspectVideo(const Uri& uri)
{
    auto result = m_inspector->discover(uri.string());

    if(result.hasVideoStream())
    {
        m_pipeline->add(m_source)->add(m_decodebin)->add(m_videoConverter)->add(m_queue)->add(m_videoScale)->add(m_videoSink)->add(m_capsfilter);
        m_source->link(m_decodebin);
        m_videoConverter->link(m_queue)->link(m_videoScale)->link(m_capsfilter)->link(m_videoSink);
    }

    if(result.hasAudioStream())
    {
        m_pipeline->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
        m_audioConverter->link(m_volume)->link(m_audioSink);
    }
}

void VideoPlayer::init()
{
    auto sink = GST_XIBOVIDEOSINK(m_videoSink->getHandler());
    gst_xibovideosink_set_handler(sink, &m_videoWindow);

    m_queue->setMaxSizeBuffers(DEFAULT_VIDEO_BUFFER);
    m_pipeline->addBusWatch(sigc::mem_fun(*this, &VideoPlayer::busMessageWatch));

    m_decodebin->signalPadAdded().connect(sigc::mem_fun(*this, &VideoPlayer::onPadAdded));
    m_decodebin->signalNoMorePads().connect(sigc::mem_fun(*this, &VideoPlayer::noMorePads));
}

bool VideoPlayer::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            Log::error("{}", err.getText());
            Log::error("[VideoHandler] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            Log::debug("[VideoHandler] End of stream");
            m_pipeline->setState(Gst::State::NULL_STATE);
            pushEvent(DurationExpiredEvent{});
            break;
        }
        default:
            break;
    }
    return true;
}

void VideoPlayer::noMorePads()
{
    Log::debug("[VideoHandler] No more pads");
}

void VideoPlayer::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    Gst::RefPtr<Gst::Pad> sinkpad;
    Log::debug("[VideoHandler] Pad added");

    auto mediaType = pad->mediaType();
    if(mediaType == Gst::MediaType::Video)
    {
        Log::debug("[VideoHandler] Video pad");

        sinkpad = m_videoConverter->getStaticPad("sink");
        pad->link(sinkpad);
    }
    else if(mediaType == Gst::MediaType::Audio)
    {
        Log::debug("[VideoHandler] Audio pad");

        sinkpad = m_audioConverter->getStaticPad("sink");
        pad->link(sinkpad);
    }
}

void VideoPlayer::load(const Uri& uri)
{
    m_source->setLocation(uri.path());

    inspectVideo(uri);
}

void VideoPlayer::play()
{
    m_pipeline->setState(Gst::State::PLAYING);
}

void VideoPlayer::stop()
{
    m_pipeline->setState(Gst::State::NULL_STATE);
}

void VideoPlayer::setVolume(int volume)
{
    m_volume->setVolume(volume / static_cast<double>(MAX_VOLUME));
}
