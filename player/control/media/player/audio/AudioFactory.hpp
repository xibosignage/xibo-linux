#pragma once

#include "control/media/MediaFactory.hpp"
#include "control/media/player/MediaPlayerOptions.hpp"

class AudioFactory : public MediaFactory
{
public:
    AudioFactory(const MediaPlayerOptions& options);

    std::unique_ptr<Media> createModel() override;

private:
    MediaPlayerOptions m_options;

};
