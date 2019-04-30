#pragma once

#include "control/media/MediaFactory.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

class MediaPlayer;

class AudioFactory : public MediaFactory
{
public:
    AudioFactory(const MediaPlayerOptions& options);

    std::unique_ptr<Media> create() override;

private:
    std::unique_ptr<MediaPlayer> createPlayer();

private:
    MediaPlayerOptions m_options;

};
