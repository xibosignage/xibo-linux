#include "MediaController.hpp"

#include "constants.hpp"

#include "utils/TimerProvider.hpp"
#include "controller/AudioController.hpp"

MediaController::MediaController(const std::shared_ptr<MediaModel>& model) :
    m_model(model),
    m_timer(std::make_unique<TimerProvider>())
{
    if(auto audio = m_model->attachedAudio())
    {
        m_attachedMedia = std::make_unique<AudioController>(audio);
    }
}

void MediaController::start()
{
    startAttachedMedia();
    startTimer(m_model->duration());
    doStart();
}

void MediaController::startTimer(int duration)
{
    if(duration > 0)
    {
        m_timer->startOnce(static_cast<unsigned int>(duration) * MSECS, [this]{
            pushEvent(DurationExpiredEvent{});
        });
    }
}

void MediaController::startAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->start();
    }
}

// FIXME check timer (maybe it should be stopped)
void MediaController::stop()
{
    stopAttachedMedia();
    doStop();
}

void MediaController::stopAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->stop();
    }
}
