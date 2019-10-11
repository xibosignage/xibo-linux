#pragma once

#include "control/widgets/render/OutputWindow.hpp"

#include <boost/signals2/signal.hpp>

class Uri;
using SignalPlaybackFinished = boost::signals2::signal<void()>;

namespace Xibo
{
    class MediaPlayer
    {
    public:
        virtual ~MediaPlayer() = default;

        virtual void load(const Uri& uri) = 0;
        virtual void setVolume(int volume) = 0;

        virtual void showOutputWindow() = 0;
        virtual void hideOutputWindow() = 0;
        virtual void setOutputWindow(const std::shared_ptr<OutputWindow>& outputWindow) = 0;
        virtual const std::shared_ptr<OutputWindow>& outputWindow() const = 0;

        virtual void play() = 0;
        virtual void stop() = 0;

        virtual SignalPlaybackFinished& playbackFinished() = 0;
    };
}
