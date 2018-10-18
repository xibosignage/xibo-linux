#pragma once

#include "IMedia.hpp"
#include "constants.hpp"

#include <glibmm/main.h>
#include <sigc++/signal.h>

template<typename Interface>
class Media : public Interface
{
public:
    static_assert(std::is_base_of<IMedia, Interface>::value, "Interface should implement IMedia");

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

template<typename Interface>
void Media<Interface>::stop()
{
    stopAudio();
    doStop();
}

template<typename Interface>
void Media<Interface>::stopAudio()
{
    if(m_audio)
    {
        m_audio->stop();
    }
}

template<typename Interface>
void Media<Interface>::start()
{
    startTimer();
    startAudio();

    doStart();
}

template<typename Interface>
void Media<Interface>::startAudio()
{
    if(m_audio)
    {
        m_audio->start();
    }
}

template<typename Interface>
void Media<Interface>::startTimer()
{
    Glib::signal_timeout().connect_once([=](){
        m_mediaTimeout.emit();
    }, duration() * MSECS);
}

template<typename Interface>
void Media<Interface>::attachAudio(std::unique_ptr<IMedia>&& audio)
{
    m_audio = std::move(audio);
}

template<typename Interface>
void Media<Interface>::connect(OnMediaTimeout callback)
{
    m_mediaTimeout.connect(callback);
}

template<typename Interface>
int Media<Interface>::duration() const
{
    return m_duration;
}

template<typename Interface>
void Media<Interface>::setDuration(int duration)
{
    m_duration = duration;
}

template<typename Interface>
sigc::signal<void>& Media<Interface>::mediaTimeout()
{
    return m_mediaTimeout;
}
