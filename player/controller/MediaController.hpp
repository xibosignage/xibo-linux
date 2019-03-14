#pragma once

#include "utils/ITimerProvider.hpp"
#include "events/EventPublisher.hpp"

#include "model/MediaModel.hpp"

#include <memory>

class MediaController : public EventPublisher<>
{
public:
    MediaController(const std::shared_ptr<MediaModel>& model);
    virtual ~MediaController() = default;

    void start();
    void stop();

protected:
    virtual void doStart() = 0;
    virtual void doStop() = 0;

private:
    void startTimer(int duration);
    void startAttachedMedia();
    void stopAttachedMedia();

private:
    std::shared_ptr<MediaModel> m_model;
    std::unique_ptr<MediaController> m_attachedMedia;
    std::unique_ptr<ITimerProvider> m_timer;

};
