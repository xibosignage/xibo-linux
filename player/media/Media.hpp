#pragma once

#include "IMedia.hpp"
#include "constants.hpp"

#include <glibmm/main.h>
#include <sigc++/signal.h>

using MediaTimeoutSignal = sigc::signal<void>;

template<typename Interface>
class Media : public Interface
{
public:
    static_assert(std::is_base_of<IMedia, Interface>::value, "Interface should implement IMedia");

    Media(int id);
    Media(const Media& other) = delete;
    Media& operator=(const Media& other) = delete;

    int id() const final;

    void stop() final;
    void start() final;

    void attachMedia(std::unique_ptr<IMedia>&& media) final;
    void connect(OnMediaTimeout callback) final;

    int duration() const final;
    void setDuration(int duration) final;

protected:
    virtual void doStop() = 0;
    virtual void doStart() = 0;

    MediaTimeoutSignal& mediaTimeout();

private:
    void startAttachedMedia();
    void stopAttachedMedia();

private:
    int m_id;
    std::unique_ptr<IMedia> m_attachedMedia;
    MediaTimeoutSignal m_mediaTimeout;
    int m_duration;

};

template<typename Interface>
Media<Interface>::Media(int id) : m_id(id)
{
}

template<typename Interface>
int Media<Interface>::id() const
{
    return m_id;
}

template<typename Interface>
void Media<Interface>::stop()
{
    stopAttachedMedia();
    doStop();
}

template<typename Interface>
void Media<Interface>::stopAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->stop();
    }
}

template<typename Interface>
void Media<Interface>::start()
{
    startAttachedMedia();
    doStart();
}

template<typename Interface>
void Media<Interface>::startAttachedMedia()
{
    if(m_attachedMedia)
    {
        m_attachedMedia->start();
    }
}

template<typename Interface>
void Media<Interface>::attachMedia(std::unique_ptr<IMedia>&& media)
{
    m_attachedMedia = std::move(media);
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
MediaTimeoutSignal& Media<Interface>::mediaTimeout()
{
    return m_mediaTimeout;
}
