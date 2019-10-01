#pragma once

#include "control/media/player/GstMediaPlayer.hpp"

class GstVideoPlayer : public GstMediaPlayer
{
protected:
    void createPipeline() override;
};
