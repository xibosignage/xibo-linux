#include "AudioFactory.hpp"

#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/MediaPlayer.hpp"

AudioFactory::AudioFactory(const MediaPlayerOptions& options) :
    m_options(options)
{
}

std::unique_ptr<Media> AudioFactory::createModel()
{
    return std::make_unique<PlayableMedia>(m_options, std::make_unique<MediaPlayer>());
}
