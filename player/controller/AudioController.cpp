#include "AudioController.hpp"

AudioController::AudioController(const std::shared_ptr<AudioModel>& model) :
    MediaController(model), m_model(model)
{
    m_player.setVolume(m_model->muted() == AudioModel::Mute::Enable ? MIN_VOLUME : m_model->volume());
    m_player.load(m_model->uri());

    subscribe(EventType::DurationExpired, [this](const Event&){
        m_player.stop();
    });

    m_player.subscribe(EventType::DurationExpired, [this](const Event&){
        if(m_model->looped() == AudioModel::Loop::Enable)
        {
            m_player.play();
        }
    });
}

void AudioController::doStart()
{
    m_player.play();
}

void AudioController::doStop()
{
    m_player.stop();
}
