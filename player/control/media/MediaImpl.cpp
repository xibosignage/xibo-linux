#include "control/media/MediaImpl.hpp"
#include "constants.hpp"

MediaImpl::MediaImpl(const MediaOptions& options) :
    options_(options),
    timer_(std::make_unique<Timer>()),
    enableStat_(false)
{
    if (options_.statPolicy != MediaOptions::StatPolicy::Inherit)
    {
        enableStat_ = options_.statPolicy == MediaOptions::StatPolicy::On ? true : false;
    }
}

void MediaImpl::setWidget(const std::shared_ptr<Xibo::Widget>& widget)
{
    widget_ = widget;
}

void MediaImpl::attach(std::unique_ptr<Media>&& attachedMedia)
{
    attachedMedia_ = std::move(attachedMedia);
}

void MediaImpl::start()
{
    if (enableStat_)
    {
        stat_.started = DateTime::now();
        stat_.id = options_.id;
    }

    startAttachedMedia();
    startTimer(options_.duration);
    onStarted();
}

void MediaImpl::startTimer(int duration)
{
    if (duration > 0)
    {
        timer_->start(std::chrono::seconds(duration), [this] { finished_(); });
    }
}

void MediaImpl::sendStat()
{
    if (enableStat_)
    {
        stat_.finished = DateTime::now();

        statReady_(stat_);

        stat_.clear();
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
    sendStat();
    stopAttachedMedia();
    onStopped();
}

void MediaImpl::enableStat(bool enable)
{
    enableStat_ = enable;
}

MediaOptions::StatPolicy MediaImpl::statPolicy() const
{
    return options_.statPolicy;
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
