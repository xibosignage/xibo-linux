#include "AudioFactory.hpp"

#include "common/constants.hpp"
#include "control/media/player/GstMediaPlayer.hpp"
#include "control/media/player/PlayableMedia.hpp"

std::unique_ptr<Xibo::Media> AudioFactory::create(const MediaPlayerOptions& options)
{
    return std::make_unique<PlayableMedia>(options, createPlayer(options));
}

std::unique_ptr<Xibo::MediaPlayer> AudioFactory::createPlayer(const MediaPlayerOptions& options)
{
    auto player = std::make_unique<GstMediaPlayer>();

    player->setVolume(options.volume);
    player->load(options.uri);

    return player;
}
