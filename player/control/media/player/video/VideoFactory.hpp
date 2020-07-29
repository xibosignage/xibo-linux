#pragma once

#include "control/media/Media.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

#include <memory>

class VideoFactory
{
public:
    std::unique_ptr<Xibo::Media> create(const MediaPlayerOptions& options, int width, int height);

private:
    std::unique_ptr<Xibo::MediaPlayer> createPlayer(const MediaPlayerOptions& options, int width, int height);
};
