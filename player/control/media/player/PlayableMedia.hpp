#pragma once

#include "control/media/MediaImpl.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

class PlayableMedia : public MediaImpl
{
public:
    PlayableMedia(const MediaPlayerOptions& options, std::unique_ptr<Xibo::MediaPlayer>&& player);

protected:
    void onStarted() override;
    void onStopped() override;

private:
    void onPlaybackFinished(const MediaPlayerOptions& options);
    void onMediaFinished();

private:
    std::unique_ptr<Xibo::MediaPlayer> player_;
};
