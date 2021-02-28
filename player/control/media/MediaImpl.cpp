#include "control/media/MediaImpl.hpp"
#include "common/constants.hpp"

MediaImpl::MediaImpl(const MediaOptions& options) :
    options_(options),
    timer_(std::make_unique<Timer>()),
    playing_(false)
{
    assert(timer_);
}

void MediaImpl::setWidget(const std::shared_ptr<Xibo::Widget>& widget)
{
    widget_ = widget;
}

void MediaImpl::attach(std::unique_ptr<Media>&& attachedMedia)
{
    attachedMedia_ = std::move(attachedMedia);
}

bool MediaImpl::playing() const
{
    return playing_;
}

void MediaImpl::start()
{
    if (playing_) return;

    playing_ = true;

    if (options_.statEnabled)
    {
        interval_.clear();
        interval_.started = DateTime::now();
    }

    startTimer(options_.duration);
    startAttachedMedia();

    onStarted();
}

void MediaImpl::startTimer(int duration)
{
    if (duration > 0)
    {
        timer_->startOnce(std::chrono::seconds(duration), [this] { finished_(); });
    }
}

void MediaImpl::startAttachedMedia()
{
    if (attachedMedia_)
    {
        attachedMedia_->start();
    }
}

void MediaImpl::onStarted()
{
    if (widget_)
    {
        widget_->show();
    }
}

void MediaImpl::stop()
{
    if (!playing_) return;

    playing_ = false;

    if (options_.statEnabled)
    {
        interval_.finished = DateTime::now();
        statReady_(interval_);
    }

    timer_->stop();
    stopAttachedMedia();

    onStopped();
}

bool MediaImpl::statEnabled() const
{
    return options_.statEnabled;
}

int MediaImpl::id() const
{
    return options_.id;
}

void MediaImpl::inTransition(std::unique_ptr<TransitionExecutor>&& transition)
{
    inTransition_ = std::move(transition);
}

void MediaImpl::outTransition(std::unique_ptr<TransitionExecutor>&& transition)
{
    outTransition_ = std::move(transition);
}

void MediaImpl::stopAttachedMedia()
{
    if (attachedMedia_)
    {
        attachedMedia_->stop();
    }
}

void MediaImpl::applyInTransition()
{
    if (inTransition_)
    {
        inTransition_->apply();
    }
}

void MediaImpl::onStopped()
{
    if (widget_)
    {
        widget_->hide();
    }
}

SignalMediaFinished& MediaImpl::finished()
{
    return finished_;
}

SignalMediaStatReady& MediaImpl::statReady()
{
    return statReady_;
}

MediaGeometry::Align MediaImpl::align() const
{
    return options_.geometry.align;
}

MediaGeometry::Valign MediaImpl::valign() const
{
    return options_.geometry.valign;
}

std::shared_ptr<Xibo::Widget> MediaImpl::view()
{
    return widget_;
}
