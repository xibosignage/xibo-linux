#pragma once

#include "IMediaPlayer.hpp"
#include "MediaPlayerOptions.hpp"
#include "control/media/Media.hpp"

class PlayableMedia : public Media
{
public:
    PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<IMediaPlayer>&& player);

protected:
    void onStarted() override;
    void onStopped() override;

private:
    void onMediaFinished();
    void onPlaybackFinished(const MediaPlayerOptions& options);

private:
    std::unique_ptr<IMediaPlayer> m_player;
};
