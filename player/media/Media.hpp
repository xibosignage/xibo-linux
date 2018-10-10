#pragma once

#include "IMedia.hpp"

#include <sigc++/signal.h>

class Media : public IMedia
{
public:
    Media() = default;

    Media(const Media& other) = delete;
    Media& operator=(const Media& other) = delete;

    void stop() final;
    void start() final;
    void startTimer() override;

    void attachAudio(std::unique_ptr<IMedia>&& audio) final;
    void connect(OnMediaTimeout callback) final;

    int duration() const final;
    void setDuration(int duration) final;

protected:
    virtual void doStop() = 0;
    virtual void doStart() = 0;

    sigc::signal<void>& mediaTimeout();

private:
    void startAudio();
    void stopAudio();

private:
    std::unique_ptr<IMedia> m_audio;
    sigc::signal<void> m_mediaTimeout;
    int m_duration;

};
