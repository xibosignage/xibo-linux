#pragma once

#include "control/media/Media.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

#include <memory>

class AudioFactory
{
public:
    std::unique_ptr<Xibo::Media> create(const MediaPlayerOptions& options);

private:
    std::unique_ptr<Xibo::MediaPlayer> createPlayer(const MediaPlayerOptions& options);
};
