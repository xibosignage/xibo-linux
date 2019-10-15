#include "GstVideoPlayer.hpp"

#include "control/media/player/gst/AudioConvert.hpp"
#include "control/media/player/gst/AutoAudioSink.hpp"
#include "control/media/player/gst/Capsfilter.hpp"
#include "control/media/player/gst/Decodebin.hpp"
#include "control/media/player/gst/Pipeline.hpp"
#include "control/media/player/gst/Queue.hpp"
#include "control/media/player/gst/UriSrc.hpp"
#include "control/media/player/gst/VideoConvert.hpp"
#include "control/media/player/gst/VideoScale.hpp"
#include "control/media/player/gst/Volume.hpp"

#include "control/widgets/render/OutputWindowGtk.hpp"

void GstVideoPlayer::createPipeline()
{
    videoConverter_ = Gst::VideoConvert::create();
    videoScale_ = Gst::VideoScale::create();
    videoSink_ = Gst::Element::create("gtksink");
    queue_ = Gst::Queue::create();
    capsfilter_ = Gst::Capsfilter::create();
    audioConverter_ = Gst::AudioConvert::create();
    audioSink_ = Gst::AutoAudioSink::create();

    if (!videoConverter_ || !videoScale_ || !videoSink_ || !queue_ || !capsfilter_ || !audioConverter_ || !audioSink_)
        throw GstMediaPlayer::Error{"GstVideoPlayer", "Error while creating pipeline"};

    GstMediaPlayer::setOutputWindow(createInternalWindow());

    pipeline_->add(source_)->add(decodebin_);
    source_->link(decodebin_);

    pipeline_->add(videoConverter_)->add(queue_)->add(videoScale_)->add(videoSink_)->add(capsfilter_);
    videoConverter_->link(queue_)->link(videoScale_)->link(capsfilter_)->link(videoSink_);

    pipeline_->add(audioConverter_)->add(volume_)->add(audioSink_);
    audioConverter_->link(volume_)->link(audioSink_);
}

std::shared_ptr<Xibo::OutputWindow> GstVideoPlayer::createInternalWindow()
{
    GtkWidget* widget = nullptr;
    g_object_get(videoSink_->handler(), "widget", &widget, nullptr);
    if (widget)
    {
        return std::make_shared<OutputWindowGtk>(Glib::wrap(widget));
    }
    return nullptr;
}
