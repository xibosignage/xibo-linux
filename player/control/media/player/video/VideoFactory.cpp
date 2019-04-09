#include "VideoFactory.hpp"

#include "control/media/player/PlayableMedia.hpp"
#include "control/media/player/MediaPlayer.hpp"

VideoFactory::VideoFactory(const MediaPlayerOptions& options) :
    m_options(options)
{
}

std::unique_ptr<Media> VideoFactory::createModel(const std::shared_ptr<Widget>& view)
{
    auto videoPlayer = std::make_unique<MediaPlayer>();
    videoPlayer->setOutputWindow(view);

    return std::make_unique<PlayableMedia>(m_options, std::move(videoPlayer));
}

ViewInfo VideoFactory::createView(int width, int height)
{
    return {std::make_shared<VideoWindow>(width, height), m_options.geometry.align, m_options.geometry.valign};
}
