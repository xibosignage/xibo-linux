#include "VideoController.hpp"

#include "constants.hpp"

VideoController::VideoController(const std::shared_ptr<VideoModel>& model,
                                 const std::shared_ptr<DrawingArea>& videoWindow) :
    VisibleMediaController(model, videoWindow),
    m_model(model),
    m_player(*videoWindow)
{
    m_player.setVolume(m_model->muted() == VideoModel::Mute::Enable ? MIN_VOLUME : MAX_VOLUME);
    m_player.load(m_model->uri());

    subscribe(EventType::DurationExpired, [this](const Event&){
        m_player.stop();
    });

    m_player.subscribe(EventType::DurationExpired, [this](const Event&){
        if(m_model->duration() == 0)
        {
            pushEvent(DurationExpiredEvent{});
            return;
        }

        if(m_model->looped() == VideoModel::Loop::Enable)
        {
            m_player.play();
        }
    });

}

void VideoController::doStart()
{
    VisibleMediaController::doStart();

    m_player.play();
}

void VideoController::doStop()
{
    VisibleMediaController::doStop();

    m_player.stop();
}
