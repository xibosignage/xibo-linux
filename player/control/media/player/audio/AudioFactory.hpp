#pragma once

#include "control/media/MediaFactory.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

class IMediaPlayer;

class AudioFactory : public MediaFactory
{
protected:
    std::unique_ptr<IMedia> create(const MediaOptions& baseOptions, const ExtraOptions& options) override;

private:
    MediaPlayerOptions createPlayerOptions(const MediaOptions& baseOptions, const ExtraOptions& options);
    std::unique_ptr<IMediaPlayer> createPlayer(const MediaPlayerOptions& options);
};
