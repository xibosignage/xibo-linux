#include "Video.hpp"
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
#include "customsink/XiboVideoSink.hpp"

const double MIN_GST_VOLUME = 0.0;
const double MAX_GST_VOLUME = 1.0;
const int DEFAULT_VIDEO_BUFFER = 500;

namespace ph = std::placeholders;

Video::Video(int id, int width, int height, int duration, const std::string& uri, bool muted, bool looped) :
    Media(id, width, height, duration, Render::Native, uri), m_muted(muted), m_looped(looped),
    m_videoFmt{"video/x-raw, width = (int)%1%, height = (int)%2%"}
{
    gst_init(nullptr, nullptr);
    m_logger = spdlog::get(LOGGER);

    if(!gst_plugin_register_static(GST_VERSION_MAJOR, GST_VERSION_MINOR, "xibovideosink", "Video Sink Plugin for gstreamer",
                                   pluginInit, "0.1", "GPL", "source", "package", "http://github.com/Stivius"))
    {
        throw std::runtime_error("XiboVideoSink was not registered");
    }

    m_pipeline = Gst::Pipeline::create("pipeline");
    m_source = Gst::FileSrc::create();
    m_decodebin = Gst::Decodebin::create();
    m_videoConverter = Gst::VideoConvert::create();
    m_audioConverter = Gst::AudioConvert::create();
    m_volume = Gst::Volume::create();
    m_videoScale = Gst::VideoScale::create();
    m_audioSink = Gst::AutoAudioSink::create();
    m_queue = Gst::Queue::create();
    m_videoSink = Gst::Element::create("xibovideosink");
    m_capsfilter = Gst::Capsfilter::create();

    auto sink = GST_XIBOVIDEOSINK(m_videoSink->getHandler());
    gst_xibovideosink_set_handler(sink, &m_videoWindow);

    if(!m_pipeline || !m_source || !m_decodebin || !m_videoConverter || !m_videoScale ||
       !m_videoSink || !m_audioConverter || !m_volume || !m_audioSink || !m_queue || !m_capsfilter)
    {
        throw std::runtime_error("[Video] One element could not be created");
    }

    m_source->setLocation(uri);
    m_queue->setMaxSizeBuffers(DEFAULT_VIDEO_BUFFER);
    m_pipeline->addBusWatch(sigc::mem_fun(*this, &Video::busMessageWatch));

    m_capsfilter->setCaps(Gst::Caps::create((m_videoFmt % width % height).str()));

    m_pipeline->add(m_source)->add(m_decodebin)->add(m_videoConverter)->add(m_queue)->add(m_videoScale)->add(m_videoSink)->add(m_capsfilter)->add(m_audioConverter)->add(m_volume)->add(m_audioSink);
    m_source->link(m_decodebin);
    m_videoConverter->link(m_queue)->link(m_videoScale)->link(m_capsfilter)->link(m_videoSink);
    m_audioConverter->link(m_volume)->link(m_audioSink);

    m_decodebin->signalPadAdded().connect(sigc::mem_fun(*this, &Video::onPadAdded));
    m_decodebin->signalNoMorePads().connect(sigc::mem_fun(*this, &Video::noMorePads));

    m_videoWindow.set_size_request(width, height);

    setVolume(m_muted ? MIN_GST_VOLUME : MAX_GST_VOLUME);
}

Video::~Video()
{
    m_logger->debug("[Video] Returned, stopping playback");
}

bool Video::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch(message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            m_logger->error("{}", err.getText());
            m_logger->error("[Video] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            m_logger->debug("[Video] End of stream");
            m_videoEnded = true;
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

void Video::noMorePads()
{
    auto pad = m_decodebin->getStaticPad("src_1");
    m_logger->debug("[Video] No more pads");

    if(!pad)
    {
        m_audioConverter->setState(Gst::State::NULL_STATE);
        m_volume->setState(Gst::State::NULL_STATE);
        m_audioSink->setState(Gst::State::NULL_STATE);
        m_pipeline->remove(m_audioConverter)->remove(m_volume)->remove(m_audioSink);
    }
}

void Video::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    Gst::RefPtr<Gst::Pad> sinkpad;
    m_logger->debug("[Video] Pad added");

    // src_0 for video stream
    auto video_pad = m_decodebin->getStaticPad("src_0");
    // src1 for audio stream
    auto audio_pad = m_decodebin->getStaticPad("src_1");

    if(video_pad && !audio_pad)
    {
        m_logger->debug("[Video] Video pad");

        auto caps = video_pad->getCurrentCaps();
        if(caps)
        {
            auto strct = caps->getStructure(0);
            m_logger->info("[Video] width: {} height: {}", strct->getWidth(), strct->getHeight());
        }

        sinkpad = m_videoConverter->getStaticPad("sink");
        pad->link(sinkpad);
    }
    else if(audio_pad)
    {
        m_logger->debug("[Video] Audio pad");
        sinkpad = m_audioConverter->getStaticPad("sink");
        pad->link(sinkpad);
    }
}

void Video::setVolume(double volume)
{
    m_volume->setVolume(volume);
}

// FIXME refactoring needed
void Video::play()
{
    if(m_videoEnded)
    {
        m_videoEnded = false;
        m_pipeline->setState(Gst::State::NULL_STATE);
        m_logger->debug("[Video] Restarting");
    }
    else
    {
        m_logger->debug("[Video] Starting");
    }
    m_pipeline->setState(Gst::State::PLAYING);
}

void Video::stop()
{
    Media::stop();
    m_videoWindow.hide();
    m_logger->debug("[Video] Stopped");
    m_pipeline->setState(Gst::State::NULL_STATE);
    m_videoEnded = true;
}

void Video::start()
{
    Media::start();
    m_videoWindow.show();
    play();
}

void Video::startTimer()
{
    if(duration())
    {
        Media::startTimer();
    }
}

void Video::setSize(int width_, int height_)
{
    if(width_ != width() || height_ != height())
    {
        Media::setSize(width_, height_);
        spdlog::get(LOGGER)->debug("set size {} {}", width_, height_);

        m_capsfilter->setCaps(Gst::Caps::create((m_videoFmt % width_ % height_).str()));

        m_videoWindow.set_size_request(width_, height_);
    }
}

void Video::requestHandler()
{
    handlerRequested().emit(m_videoWindow, DEFAULT_LEFT_POS, DEFAULT_TOP_POS);
}

bool Video::looped() const
{
    return m_looped;
}

bool Video::muted() const
{
    return m_muted;
}
