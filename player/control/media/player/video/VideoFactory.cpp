#include "VideoFactory.hpp"

#include "constants.hpp"
#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/video/GstVideoPlayer.hpp"

std::unique_ptr<Xibo::Media> VideoFactory::create(const MediaPlayerOptions& options, int width, int height)
{
    auto videoPlayer = createPlayer(options, width, height);
    return std::make_unique<PlayableMedia>(options, std::move(videoPlayer));
}

std::unique_ptr<Xibo::MediaPlayer> VideoFactory::createPlayer(const MediaPlayerOptions& options, int width, int height)
{
    auto location = options.uri.scheme() == Uri::FileScheme ? MediaPlayerOptions::Local : MediaPlayerOptions::Network;
    auto player = GstMediaPlayer::create<GstVideoPlayer>(location);

    player->setVolume(options.muted == MediaPlayerOptions::Mute::Enable ? MinVolume : MaxVolume);
    player->load(options.uri);

    // TODO set window explicitly if no internal window created
    if (auto output = player->outputWindow())
    {
        output->setSize(width, height);
    }

    return player;
}
