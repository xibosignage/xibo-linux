#pragma once

#include "MediaPlayerOptions.hpp"
#include "MediaPlayer.hpp"

class Media;

class MediaPlayerController
{
public:
    MediaPlayerController(const MediaPlayerOptions& options, Media& media, std::unique_ptr<MediaPlayer>&& player);

private:
    Media& m_media;
    std::unique_ptr<MediaPlayer> m_player;

};
