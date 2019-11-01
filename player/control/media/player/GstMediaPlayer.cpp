#include "GstMediaPlayer.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"
#include "constants.hpp"
#include "control/widgets/render/OutputWindowGtk.hpp"

#include <gst/gst.h>

GstMediaPlayer::GstMediaPlayer() :
    playbin_(gst_element_factory_make("playbin", "playbin")),
    videoSink_(gst_element_factory_make("gtksink", "videosink")),
    playbinBus_(nullptr)
{
    if (!playbin_) throw Error{"GstMediaPlayer", "Unable to create player: playbin is missing."};
    if (!videoSink_) throw Error{"GstMediaPlayer", "Unable to create player: gtksink is missing."};

    g_object_set(playbin_, "video-sink", videoSink_, nullptr);

    GtkWidget* widget = nullptr;
    g_object_get(videoSink_, "widget", &widget, nullptr);
    if (widget)
    {
        // take ownership and destroy in the container later
        outputWindow_ = std::make_shared<OutputWindowGtk>(Glib::wrap(widget));
    }

    playbinBus_ = gst_element_get_bus(playbin_);
    gst_bus_add_watch(playbinBus_, static_cast<GstBusFunc>(&GstMediaPlayer::busMessageWatch), this);
}

GstMediaPlayer::~GstMediaPlayer()
{
    gst_object_unref(playbinBus_);
    gst_element_set_state(playbin_, GST_STATE_NULL);
    gst_object_unref(playbin_);  // videoSink_ should be unrefed as a child
}

void GstMediaPlayer::load(const Uri& uri)
{
    g_object_set(playbin_, "uri", uri.string().c_str(), nullptr);
}

void GstMediaPlayer::setVolume(int volume)
{
    checkVolume(volume);
    g_object_set(playbin_, "volume", volume / static_cast<double>(MaxVolume), nullptr);
}

void GstMediaPlayer::setAspectRatio(MediaGeometry::ScaleType scaleType)
{
    bool aspectRatio = scaleType == MediaGeometry::ScaleType::Scaled ? true : false;
    g_object_set(playbin_, "force-aspect-ratio", aspectRatio, nullptr);
}

void GstMediaPlayer::checkVolume(int volume)
{
    if (volume < MinVolume || volume > MaxVolume) throw Error{"GstMediaPlayer", "Volume should be in [0-100] range"};
}

void GstMediaPlayer::play()
{
    gst_element_set_state(playbin_, GST_STATE_PLAYING);
}

void GstMediaPlayer::stop()
{
    gst_element_set_state(playbin_, GST_STATE_NULL);
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

gboolean GstMediaPlayer::busMessageWatch(GstBus* /*bus*/, GstMessage* msg, gpointer data)
{
    switch (msg->type)
    {
        case GST_MESSAGE_EOS:
        {
            assert(data);
            auto player = reinterpret_cast<GstMediaPlayer*>(data);

            Log::debug("[GstMediaPlayer] End of stream");
            gst_element_set_state(player->playbin_, GST_STATE_NULL);
            player->playbackFinished_();
            break;
        }
        case GST_MESSAGE_ERROR:
        {
            GError* err = nullptr;
            gchar* debug_info = nullptr;

            gst_message_parse_error(msg, &err, &debug_info);
            Log::error("[GstMediaPlayer] Error from element {}: {}", msg->src->name, err->message);
            if (debug_info)
            {
                Log::debug("[GstMediaPlayer] Debug details: {}", debug_info);
            }
            g_clear_error(&err);
            g_free(debug_info);
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
