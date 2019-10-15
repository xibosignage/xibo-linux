#include "PlayableMedia.hpp"

PlayableMedia::PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<Xibo::MediaPlayer>&& player) :
    MediaImpl(options),
    player_(std::move(player))
{
    assert(player_);

    MediaImpl::setWidget(player_->outputWindow());
    MediaImpl::finished().connect(std::bind(&PlayableMedia::onMediaFinished, this));
    player_->playbackFinished().connect(std::bind(&PlayableMedia::onPlaybackFinished, this, options));
}

void PlayableMedia::onStarted()
{
    player_->showOutputWindow();
    player_->play();
}

void PlayableMedia::onStopped()
{
    player_->hideOutputWindow();
    player_->stop();
}

void PlayableMedia::onMediaFinished()
{
    player_->stop();
}

void PlayableMedia::onPlaybackFinished(const MediaPlayerOptions& options)
{
    if (options.duration == 0)
    {
        finished()();
        return;
    }

    if (options.looped == MediaPlayerOptions::Loop::Enable)
    {
        player_->play();
    }
}
