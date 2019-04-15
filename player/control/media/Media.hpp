#pragma once

#include "utils/ITimerProvider.hpp"
#include "MediaOptions.hpp"

#include <memory>
#include <sigc++/sigc++.h>

using SignalStarted = sigc::signal<void()>;
using SignalStopped = sigc::signal<void()>;
using SignalMediaFinished = sigc::signal<void()>;

class Media
{
public:
    Media(const MediaOptions& options);
    virtual ~Media() = default;

    void attachMedia(std::unique_ptr<Media>&& attachedMedia);
    void start();
    void stop();

    SignalStarted started();
    SignalStopped stopped();
    SignalStopped mediaFinished();

private:
    void startTimer(int duration);
    void startAttachedMedia();
    void stopAttachedMedia();

private:
    MediaOptions m_options;
    std::unique_ptr<Media> m_attachedMedia;
    std::unique_ptr<ITimerProvider> m_timer;
    SignalStarted m_started;
    SignalStopped m_stopped;
    SignalMediaFinished m_mediaFinished;

};
