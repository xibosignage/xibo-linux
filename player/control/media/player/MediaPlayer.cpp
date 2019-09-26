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
    pipeline_ = Gst::Pipeline::create();
    decodebin_ = Gst::Decodebin::create();

    videoConverter_ = Gst::VideoConvert::create();
    queue_ = Gst::Queue::create();
    videoScale_ = Gst::VideoScale::create();
    videoSink_ = Gst::Element::create("xibovideosink");
    capsfilter_ = Gst::Capsfilter::create();
    inspector_ = Gst::Inspector::create(InspectorTimeout);

    audioConverter_ = Gst::AudioConvert::create();
    volume_ = Gst::Volume::create();
    audioSink_ = Gst::AutoAudioSink::create();

    if (!pipeline_ || !decodebin_ || !videoConverter_ || !videoScale_ || !videoSink_ || !queue_ || !capsfilter_ ||
        !audioConverter_ || !volume_ || !audioSink_)
    {
        throw std::runtime_error("[MediaPlayer] Error during creation");
    }
}

void MediaPlayer::init()
{
    pipeline_->addBusWatch(std::bind(&MediaPlayer::busMessageWatch, this, ph::_1));

    decodebin_->signalPadAdded().connect(std::bind(&MediaPlayer::onPadAdded, this, ph::_1));
    decodebin_->signalNoMorePads().connect(std::bind(&MediaPlayer::noMorePads, this));
}

void MediaPlayer::setOutputWindow(const std::shared_ptr<IVideoWindow>& window)
{
    outputWindow_ = window;

    outputWindow_->shown().connect([this]() {
        boost::format videoFmt{"video/x-raw, width = (int)%1%, height = (int)%2%"};
        capsfilter_->setCaps(Gst::Caps::create((videoFmt % outputWindow_->width() % outputWindow_->height()).str()));
    });

    auto sink = GST_XIBOVIDEOSINK(videoSink_->handler());
    gst_xibovideosink_set_handler(sink, outputWindow_);
}

std::shared_ptr<IVideoWindow> MediaPlayer::outputWindow() const
{
    return outputWindow_;
}

void MediaPlayer::load(const Uri& uri)
{
    source_ = Gst::UriSrc::create(uri.scheme());
    source_->setLocation(uri);

    inspectFile(uri);
}

void MediaPlayer::inspectFile(const Uri& uri)
{
    mediaInfo_ = inspector_->discover(uri.string());

    pipeline_->add(source_)->add(decodebin_);
    source_->link(decodebin_);

    if (mediaInfo_.videoStream)
    {
        pipeline_->add(videoConverter_)->add(queue_)->add(videoScale_)->add(videoSink_)->add(capsfilter_);
        videoConverter_->link(queue_)->link(videoScale_)->link(capsfilter_)->link(videoSink_);
    }
    if (mediaInfo_.audioStream)
    {
        pipeline_->add(audioConverter_)->add(volume_)->add(audioSink_);
        audioConverter_->link(volume_)->link(audioSink_);
    }
}

void MediaPlayer::play()
{
    if (outputWindow_)
    {
        outputWindow_->show();
    }
    pipeline_->setState(Gst::State::PLAYING);
}

void MediaPlayer::stopAndRemove()
{
    if (outputWindow_)
    {
        outputWindow_->hide();
    }
    pipeline_->setState(Gst::State::NULL_STATE);
}

void MediaPlayer::stopPlayback()
{
    pipeline_->setState(Gst::State::NULL_STATE);
}

void MediaPlayer::setVolume(int volume)
{
    volume_->setVolume(volume / static_cast<double>(MaxVolume));
}

SignalPlaybackFinished& MediaPlayer::playbackFinished()
{
    return playbackFinished_;
}

Gst::InspectorResult MediaPlayer::mediaInfo() const
{
    return mediaInfo_;
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
            pipeline_->setState(Gst::State::NULL_STATE);
            playbackFinished_();
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

        auto sinkpad = videoConverter_->staticPad("sink");
        pad->link(sinkpad);
    }
    else if (mediaType == Gst::MediaType::Audio)
    {
        Log::trace("[MediaPlayer] Audio pad added");

        auto sinkpad = audioConverter_->staticPad("sink");
        pad->link(sinkpad);
    }
}
