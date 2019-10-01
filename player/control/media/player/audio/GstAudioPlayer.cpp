#include "GstAudioPlayer.hpp"

#include "control/media/player/gst/AudioConvert.hpp"
#include "control/media/player/gst/AutoAudioSink.hpp"
#include "control/media/player/gst/Decodebin.hpp"
#include "control/media/player/gst/Pipeline.hpp"
#include "control/media/player/gst/UriSrc.hpp"
#include "control/media/player/gst/Volume.hpp"

void GstAudioPlayer::createPipeline()
{
    audioConverter_ = Gst::AudioConvert::create();
    audioSink_ = Gst::AutoAudioSink::create();

    if (!audioSink_ || !audioConverter_) throw GstMediaPlayer::Error{"[GstAudioPlayer] Error while creating pipeline"};

    pipeline_->add(source_)->add(decodebin_);
    source_->link(decodebin_);

    pipeline_->add(audioConverter_)->add(volume_)->add(audioSink_);
    audioConverter_->link(volume_)->link(audioSink_);
}
