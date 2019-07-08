#pragma once

#include "control/media/creators/MediaFactory.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"
#include "gstwrapper/Inspector.hpp"

class IMediaPlayer;
class IVideoWindow;

class VideoFactory : public MediaFactory
{
protected:
    std::unique_ptr<IMedia> createImpl(const MediaOptions& baseOptions, const ExtraOptions& options) override;

private:
    MediaPlayerOptions createPlayerOptions(const MediaOptions& baseOptions, const ExtraOptions& options);
    std::unique_ptr<IMediaPlayer> createPlayer(const MediaPlayerOptions& options, int width, int height);
    std::shared_ptr<IVideoWindow> createView(int width, int height, MediaGeometry::ScaleType scaleType, const Gst::InspectorResult& info);
};
