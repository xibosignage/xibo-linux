#pragma once

#include "control/media/player/GstMediaPlayer.hpp"

class GstVideoPlayer : public GstMediaPlayer
{
public:
    void createPipeline() override;

protected:
    std::shared_ptr<Xibo::OutputWindow> createInternalWindow();
};
