#pragma once

#include "control/media/MediaFactory.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"
#include "gstwrapper/Inspector.hpp"

class MediaPlayer;
class VideoWindow;

class VideoFactory : public MediaFactory
{
public:
    VideoFactory(int width, int height, const MediaPlayerOptions& options);

    std::unique_ptr<Media> create() override;

private:
    std::unique_ptr<MediaPlayer> createPlayer(int width, int height);
    std::shared_ptr<VideoWindow> createView(int width, int height, const Gst::InspectorResult& info);

private:
    MediaPlayerOptions m_options;
    int m_width;
    int m_height;
};
