#include "VideoFactory.hpp"

#include "VideoWindow.hpp"
#include "control/media/MediaResources.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "control/media/player/MediaPlayerResources.hpp"
#include "control/media/player/PlayableMedia.hpp"

#include "constants.hpp"

std::unique_ptr<IMedia> VideoFactory::create(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    int width = std::stoi(options.at(XlfResources::Media::Width));
    int height = std::stoi(options.at(XlfResources::Media::Height));
    auto playerOptions = createPlayerOptions(baseOptions, options);

    auto videoPlayer = createPlayer(playerOptions, width, height);
    return std::make_unique<PlayableMedia>(playerOptions, std::move(videoPlayer));
}

MediaPlayerOptions VideoFactory::createPlayerOptions(const MediaOptions& baseOptions, const ExtraOptions& options)
{
    auto muted = static_cast<MediaPlayerOptions::Mute>(std::stoi(options.at(XlfResources::Player::Mute)));
    auto looped = static_cast<MediaPlayerOptions::Loop>(std::stoi(options.at(XlfResources::Player::Loop)));

    return MediaPlayerOptions{baseOptions, muted, looped, MaxVolume};
}

std::unique_ptr<IMediaPlayer> VideoFactory::createPlayer(const MediaPlayerOptions& options, int width, int height)
{
    auto player = std::make_unique<MediaPlayer>();

    player->setVolume(options.muted == MediaPlayerOptions::Mute::Enable ? MinVolume : MaxVolume);
    player->load(options.uri);
    player->setOutputWindow(createView(width, height, options.geometry.scaleType, player->mediaInfo()));

    return player;
}

std::shared_ptr<IVideoWindow> VideoFactory::createView(int width, int height, MediaGeometry::ScaleType scaleType,
                                                       const Gst::InspectorResult& info)
{
    auto view = std::make_shared<VideoWindow>(width, height);

    if (scaleType == MediaGeometry::ScaleType::Scaled)
    {
        double scaleX = static_cast<double>(view->width()) / info.frameWidth;
        double scaleY = static_cast<double>(view->height()) / info.frameHeight;
        double scaleFactor = std::min(scaleX, scaleY);

        int scaledWidth = static_cast<int>(info.frameWidth * scaleFactor);
        int scaledHeight = static_cast<int>(info.frameHeight * scaleFactor);

        view->setSize(scaledWidth, scaledHeight);
    }

    return view;
}
