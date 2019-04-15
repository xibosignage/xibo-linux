#pragma once

#include "control/media/MediaFactory.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

#include "VideoWindow.hpp"

class VideoFactory : public VisibleMediaFactory
{
public:
    VideoFactory(const MediaPlayerOptions& options);

    std::unique_ptr<Media> createModel(const std::shared_ptr<Widget>& view) override;
    ViewInfo createView(int width, int height) override;

private:
    MediaPlayerOptions m_options;
    std::shared_ptr<VideoWindow> m_window;

};
