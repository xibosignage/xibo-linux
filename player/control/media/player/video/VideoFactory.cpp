#include "VideoFactory.hpp"

#include "constants.hpp"
#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/video/GstVideoPlayer.hpp"
#include "control/widgets/render/OutputWindowFactory.hpp"

std::unique_ptr<Xibo::Media> VideoFactory::create(const MediaPlayerOptions& options, int width, int height)
{
    auto videoPlayer = createPlayer(options, width, height);
    return std::make_unique<PlayableMedia>(options, std::move(videoPlayer));
}

std::unique_ptr<Xibo::MediaPlayer> VideoFactory::createPlayer(const MediaPlayerOptions& options, int width, int height)
{
    auto location = options.uri.scheme() == Uri::Scheme::File ? MediaPlayerOptions::Local : MediaPlayerOptions::Network;
    auto player = GstMediaPlayer::create<GstVideoPlayer>(location);

    player->setVolume(options.muted == MediaPlayerOptions::Mute::Enable ? MinVolume : MaxVolume);
    player->load(options.uri);
    player->setOutputWindow(createView(width, height, options.geometry.scaleType));

    return player;
}

std::shared_ptr<Xibo::OutputWindow> VideoFactory::createView(int width, int height, MediaGeometry::ScaleType scaleType)
{
    auto view = OutputWindowFactory::create(width, height);

    if (scaleType == MediaGeometry::ScaleType::Scaled)
    {
        //        double scaleX = static_cast<double>(view->width()) / info.frameWidth;
        //        double scaleY = static_cast<double>(view->height()) / info.frameHeight;
        //        double scaleFactor = std::min(scaleX, scaleY);

        //        int scaledWidth = static_cast<int>(info.frameWidth * scaleFactor);
        //        int scaledHeight = static_cast<int>(info.frameHeight * scaleFactor);

        //        view->setSize(scaledWidth, scaledHeight);
    }

    return view;
}
