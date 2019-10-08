#include "GstMediaPlayer.hpp"

#include "control/media/player/gst/AudioConvert.hpp"
#include "control/media/player/gst/AutoAudioSink.hpp"
#include "control/media/player/gst/Caps.hpp"
#include "control/media/player/gst/Capsfilter.hpp"
#include "control/media/player/gst/Decodebin.hpp"
#include "control/media/player/gst/Pad.hpp"
#include "control/media/player/gst/Pipeline.hpp"
#include "control/media/player/gst/Queue.hpp"
#include "control/media/player/gst/UriSrc.hpp"
#include "control/media/player/gst/VideoConvert.hpp"
#include "control/media/player/gst/VideoScale.hpp"
#include "control/media/player/gst/Volume.hpp"
#include "control/media/player/video/XiboVideoSink.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"

#include "constants.hpp"

#include <boost/format.hpp>

void GstMediaPlayer::init(MediaPlayerOptions::Location location)
{
    createBaseElements(location);
    createPipeline();

    pipeline_->addBusWatch(std::bind(&GstMediaPlayer::busMessageWatch, this, ph::_1));

    decodebin_->signalPadAdded().connect(std::bind(&GstMediaPlayer::onPadAdded, this, ph::_1));
    decodebin_->signalNoMorePads().connect(std::bind(&GstMediaPlayer::noMorePads, this));
}

void GstMediaPlayer::createBaseElements(MediaPlayerOptions::Location location)
{
    pipeline_ = Gst::Pipeline::create();
    decodebin_ = Gst::Decodebin::create();
    volume_ = Gst::Volume::create();
    source_ = Gst::UriSrc::create(static_cast<Gst::UriSrc::Type>(location));

    if (!pipeline_ || !decodebin_ || !volume_ || !source_) throw Error{"GstMediaPlayer", "Error during creation"};
}

void GstMediaPlayer::load(const Uri& uri)
{
    source_->setLocation(uri);
}

void GstMediaPlayer::setVolume(int volume)
{
    checkVolume(volume);
    volume_->setVolume(volume / static_cast<double>(MaxVolume));
}

void GstMediaPlayer::checkVolume(int volume)
{
    if (volume < MinVolume || volume > MaxVolume) throw Error{"GstMediaPlayer", "Volume should be in [0-100] range"};
}

void GstMediaPlayer::play()
{
    pipeline_->setState(Gst::State::PLAYING);
}

void GstMediaPlayer::stop()
{
    pipeline_->setState(Gst::State::NULL_STATE);
}

void GstMediaPlayer::showOutputWindow()
{
    if (outputWindow_)
    {
        outputWindow_->show();
    }
}

void GstMediaPlayer::hideOutputWindow()
{
    if (outputWindow_)
    {
        outputWindow_->hide();
    }
}

void GstMediaPlayer::setOutputWindow(const std::shared_ptr<Xibo::OutputWindow>& outputWindow)
{
    assert(outputWindow);

    outputWindow_ = outputWindow;

    outputWindow_->shown().connect([this]() {
        boost::format videoFmt{"video/x-raw, width = (int)%1%, height = (int)%2%"};
        capsfilter_->setCaps(Gst::Caps::create((videoFmt % outputWindow_->width() % outputWindow_->height()).str()));
    });

    auto sink = GST_XIBOVIDEOSINK(videoSink_->handler());
    gst_xibovideosink_set_handler(sink, outputWindow_);
}

const std::shared_ptr<Xibo::OutputWindow>& GstMediaPlayer::outputWindow() const
{
    return outputWindow_;
}

SignalPlaybackFinished& GstMediaPlayer::playbackFinished()
{
    return playbackFinished_;
}

bool GstMediaPlayer::busMessageWatch(const Gst::RefPtr<Gst::Message>& message)
{
    switch (message->type())
    {
        case Gst::MessageType::ERROR:
        {
            auto err = message->parseError();
            Log::error("[GstMediaPlayer] {}", err.getText());
            Log::debug("[GstMediaPlayer] Debug details: {}", err.getDebugInfo());
            break;
        }
        case Gst::MessageType::EOS:
        {
            Log::debug("[GstMediaPlayer] End of stream");
            pipeline_->setState(Gst::State::NULL_STATE);
            playbackFinished_();
            break;
        }
        default: break;
    }
    return true;
}

void GstMediaPlayer::noMorePads()
{
    Log::debug("[GstMediaPlayer] No more pads");
}

void GstMediaPlayer::onPadAdded(const Gst::RefPtr<Gst::Pad>& pad)
{
    auto mediaType = pad->mediaType();
    if (mediaType == Gst::MediaType::Video)
    {
        Log::debug("[GstMediaPlayer] Video pad added");

        auto sinkpad = videoConverter_->staticPad("sink");
        if (sinkpad)
        {
            pad->link(sinkpad);
        }
    }
    else if (mediaType == Gst::MediaType::Audio)
    {
        Log::debug("[GstMediaPlayer] Audio pad added");

        auto sinkpad = audioConverter_->staticPad("sink");
        if (sinkpad)
        {
            pad->link(sinkpad);
        }
    }
}
