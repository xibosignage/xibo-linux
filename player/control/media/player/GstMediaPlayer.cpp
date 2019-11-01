#include "GstMediaPlayer.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"

#include "constants.hpp"
#include "control/widgets/render/OutputWindowGtk.hpp"

#include <boost/format.hpp>
#include <gstreamermm/bus.h>
#include <gstreamermm/elementfactory.h>

GstMediaPlayer::GstMediaPlayer() :
    playbin_(Gst::PlayBin::create()),
    videoSink_(Gst::ElementFactory::create_element("gtksink"))
{
    if (!playbin_) throw Error{"GstMediaPlayer", "Unable to create player: playbin is missing."};
    if (!videoSink_) throw Error{"GstMediaPlayer", "Unable to create player: gtksink is missing."};

    playbin_->set_property("video-sink", videoSink_);

    Gtk::Widget* widget = nullptr;
    videoSink_->get_property("widget", widget);
    if (widget)
    {
        outputWindow_ = std::make_shared<OutputWindowGtk>(widget);
    }

    Glib::RefPtr<Gst::Bus> bus = playbin_->get_bus();
    bus->add_watch(sigc::mem_fun(*this, &GstMediaPlayer::busMessageWatch));
}

GstMediaPlayer::~GstMediaPlayer()
{
    playbin_->set_state(Gst::STATE_NULL);
}

void GstMediaPlayer::load(const Uri& uri)
{
    playbin_->set_property("uri", uri.string());
}

void GstMediaPlayer::setVolume(int volume)
{
    checkVolume(volume);
    playbin_->set_volume(Gst::STREAM_VOLUME_FORMAT_LINEAR, volume / static_cast<double>(MaxVolume));
}

void GstMediaPlayer::checkVolume(int volume)
{
    if (volume < MinVolume || volume > MaxVolume) throw Error{"GstMediaPlayer", "Volume should be in [0-100] range"};
}

void GstMediaPlayer::play()
{
    playbin_->set_state(Gst::STATE_PLAYING);
}

void GstMediaPlayer::stop()
{
    playbin_->set_state(Gst::STATE_NULL);
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
}

const std::shared_ptr<Xibo::OutputWindow>& GstMediaPlayer::outputWindow() const
{
    return outputWindow_;
}

bool GstMediaPlayer::busMessageWatch(const Glib::RefPtr<Gst::Bus>& /*bus*/, const Glib::RefPtr<Gst::Message>& message)
{
    switch (message->get_message_type())
    {
        case Gst::MESSAGE_EOS:
            Log::debug("[GstMediaPlayer] End of stream");
            playbin_->set_state(Gst::STATE_NULL);
            playbackFinished_();
            return false;
        case Gst::MESSAGE_ERROR:
        {
            auto messageError = Glib::RefPtr<Gst::MessageError>::cast_static(message);
            if (messageError)
            {
                auto error = messageError->parse_error();
                Log::error("[GstMediaPlayer] {}", error.what());
                Log::debug("[GstMediaPlayer] Debug details: {}", messageError->parse_debug());
            }
            else
            {
                Log::error("[GstMediaPlayer] Unknown error");
            }
            return false;
        }
        default: break;
    }

    return true;
}

SignalPlaybackFinished& GstMediaPlayer::playbackFinished()
{
    return playbackFinished_;
}
