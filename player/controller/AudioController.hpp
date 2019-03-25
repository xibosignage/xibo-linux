#pragma once

#include "MediaController.hpp"

#include "model/AudioModel.hpp"
#include "view/AudioPlayer.hpp"

class AudioController : public MediaController
{
public:
    AudioController(const std::shared_ptr<AudioModel>& model);

protected:
    void doStart() override;
    void doStop() override;

private:
    std::shared_ptr<AudioModel> m_model;
    AudioPlayer m_player;

};

