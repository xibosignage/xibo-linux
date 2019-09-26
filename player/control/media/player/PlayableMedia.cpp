#include "PlayableMedia.hpp"

#include "control/media/Media.hpp"
#include "video/IVideoWindow.hpp"

PlayableMedia::PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<IMediaPlayer>&& player) :
    Media(options, player->outputWindow()),
    player_(std::move(player))
{
    mediaFinished().connect(std::bind(&PlayableMedia::onMediaFinished, this));

    player_->playbackFinished().connect(std::bind(&PlayableMedia::onPlaybackFinished, this, options));
}

void PlayableMedia::onMediaFinished()
{
    player_->stopPlayback();
}

void PlayableMedia::onStarted()
{
    player_->play();
}

void PlayableMedia::onStopped()
{
    player_->stopAndRemove();
}

void PlayableMedia::onPlaybackFinished(const MediaPlayerOptions& options)
{
    if (options.duration == 0)
    {
        mediaFinished()();
        return;
    }

    if (options.looped == MediaPlayerOptions::Loop::Enable)
    {
        player_->play();
    }
}
