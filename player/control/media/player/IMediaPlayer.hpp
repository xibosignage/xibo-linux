#pragma once

#include "gstwrapper/Inspector.hpp"
#include <sigc++/signal.h>

class Uri;
class IVideoWindow;

using SignalPlaybackFinished = sigc::signal<void()>;

class IMediaPlayer
{
public:
    virtual ~IMediaPlayer() = default;

    virtual void setOutputWindow(const std::shared_ptr<IVideoWindow>& window) = 0;
    virtual std::shared_ptr<IVideoWindow> outputWindow() const = 0;

    virtual void load(const Uri& uri) = 0;
    virtual void setVolume(int volume) = 0;

    virtual void play() = 0;
    virtual void stopAndRemove() = 0;
    virtual void stopPlayback() = 0;

    virtual SignalPlaybackFinished playbackFinished() = 0;
    virtual Gst::InspectorResult mediaInfo() const = 0;
};
