#pragma once

#include "control/media/player/GstMediaPlayer.hpp"

class GstAudioPlayer : public GstMediaPlayer
{
protected:
    void createPipeline() override;
};
