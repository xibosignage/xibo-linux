#include "PlayableMedia.hpp"

#include "control/media/Media.hpp"
#include "video/IVideoWindow.hpp"

PlayableMedia::PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<IMediaPlayer>&& player) :
    Media(options, player->outputWindow()),
    m_player(std::move(player))
{
    mediaFinished().connect(std::bind(&PlayableMedia::onMediaFinished, this));

    m_player->playbackFinished().connect(std::bind(&PlayableMedia::onPlaybackFinished, this, options));
}

void PlayableMedia::onMediaFinished()
{
    m_player->stopPlayback();
}

void PlayableMedia::onStarted()
{
    m_player->play();
}

void PlayableMedia::onStopped()
{
    m_player->stopAndRemove();
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
        m_player->play();
    }
}
