#pragma once

#include "MediaGeometry.hpp"
#include "control/common/IWidget.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>

using SignalMediaFinished = boost::signals2::signal<void()>;
class TransitionExecutor;

class IMedia
{
public:
    virtual ~IMedia() = default;

    virtual void attach(std::unique_ptr<IMedia>&& attachedMedia) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;

    virtual void setInTransition(std::unique_ptr<TransitionExecutor>&& transition) = 0;
    virtual void setOutTransition(std::unique_ptr<TransitionExecutor>&& transition) = 0;

    virtual SignalMediaFinished& mediaFinished() = 0;

    virtual MediaGeometry::Align align() const = 0;
    virtual MediaGeometry::Valign valign() const = 0;
    virtual std::shared_ptr<IWidget> view() = 0;
};
