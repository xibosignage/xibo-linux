#include "AudioFactory.hpp"

#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/player/PlayableMedia.hpp"

#include "constants.hpp"

std::unique_ptr<IMedia> AudioFactory::create(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    auto playerOptions = createPlayerOptions(baseOptions, options);

    return std::make_unique<PlayableMedia>(playerOptions, createPlayer(playerOptions));
}

MediaPlayerOptions AudioFactory::createPlayerOptions(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    auto looped = static_cast<MediaPlayerOptions::Loop>(std::stoi(options.at(XlfResources::Player::Loop)));
    int volume = std::stoi(options.at(XlfResources::Player::Volume));

    return MediaPlayerOptions{baseOptions, MediaPlayerOptions::Mute::Disable, looped, volume};
}

std::unique_ptr<IMediaPlayer> AudioFactory::createPlayer(const MediaPlayerOptions& options)
{
    auto player = std::make_unique<MediaPlayer>();

    player->setVolume(options.volume);
    player->load(options.uri);

    return player;
}
