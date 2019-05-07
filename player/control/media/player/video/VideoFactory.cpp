#include "VideoFactory.hpp"

#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/MediaPlayer.hpp"
#include "VideoWindow.hpp"

#include "constants.hpp"

VideoFactory::VideoFactory(int width, int height, const MediaPlayerOptions& options) :
    m_options(options),
    m_width(width),
    m_height(height)
{
}

std::unique_ptr<Media> VideoFactory::create()
{
    auto videoPlayer = createPlayer(m_width, m_height);

    return std::make_unique<PlayableMedia>(m_options, std::move(videoPlayer));
}

std::unique_ptr<MediaPlayer> VideoFactory::createPlayer(int width, int height)
{
    auto player = std::make_unique<MediaPlayer>();

    player->setVolume(m_options.muted == MediaPlayerOptions::Mute::Enable ? MIN_VOLUME : MAX_VOLUME);
    player->load(m_options.uri);
    player->setOutputWindow(createView(width, height, player->mediaInfo()));

    return player;
}

std::shared_ptr<VideoWindow> VideoFactory::createView(int width, int height, const Gst::InspectorResult& info)
{
    auto view = std::make_shared<VideoWindow>(width, height);

    if(m_options.geometry.scaleType == MediaGeometry::ScaleType::Scaled)
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
