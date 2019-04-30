#include "AudioFactory.hpp"

#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/MediaPlayer.hpp"

#include "constants.hpp"

AudioFactory::AudioFactory(const MediaPlayerOptions& options) :
    m_options(options)
{
}

std::unique_ptr<Media> AudioFactory::create()
{
    return std::make_unique<PlayableMedia>(m_options, createPlayer());
}

std::unique_ptr<MediaPlayer> AudioFactory::createPlayer()
{
    auto player = std::make_unique<MediaPlayer>();

    player->setVolume(m_options.muted == MediaPlayerOptions::Mute::Enable ? MIN_VOLUME : MAX_VOLUME);
    player->load(m_options.uri);

    return player;
}
