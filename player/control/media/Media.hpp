#pragma once

#include "control/media/MediaOptions.hpp"
#include "control/widgets/Widget.hpp"
#include "stat/PlayingTime.hpp"

#include <boost/signals2/signal.hpp>

class TransitionExecutor;

using SignalMediaFinished = boost::signals2::signal<void()>;
using SignalMediaStatReady = boost::signals2::signal<void(Stats::PlayingTime)>;

namespace Xibo
{
    class Media
    {
    public:
        virtual ~Media() = default;

        virtual void setWidget(const std::shared_ptr<Widget>& widget) = 0;
        virtual void attach(std::unique_ptr<Media>&& attachedMedia) = 0;
        virtual bool playing() const = 0;
        virtual void start() = 0;
        virtual void stop() = 0;

        virtual bool statEnabled() const = 0;
        virtual int id() const = 0;

        virtual void inTransition(std::unique_ptr<TransitionExecutor>&& transition) = 0;
        virtual void outTransition(std::unique_ptr<TransitionExecutor>&& transition) = 0;

        virtual SignalMediaFinished& finished() = 0;
        virtual SignalMediaStatReady& statReady() = 0;

        virtual MediaGeometry::Align align() const = 0;
        virtual MediaGeometry::Valign valign() const = 0;
        virtual std::shared_ptr<Widget> view() = 0;
    };
}
