#include "AudioFactory.hpp"

#include "constants.hpp"
#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/audio/GstAudioPlayer.hpp"

std::unique_ptr<Xibo::Media> AudioFactory::create(const MediaPlayerOptions& options)
{
    return std::make_unique<PlayableMedia>(options, createPlayer(options));
}

std::unique_ptr<Xibo::MediaPlayer> AudioFactory::createPlayer(const MediaPlayerOptions& options)
{
    auto location = options.uri.scheme() == Uri::Scheme::File ? MediaPlayerOptions::Local : MediaPlayerOptions::Network;
    auto player = GstMediaPlayer::create<GstAudioPlayer>(location);

    player->setVolume(options.volume);
    player->load(options.uri);

    return player;
}
