#include "control/media/MediaImpl.hpp"
#include "constants.hpp"

MediaImpl::MediaImpl(const MediaOptions& options) : options_(options), timer_(std::make_unique<Timer>()) {}

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
    startAttachedMedia();
    startTimer(options_.duration);
    onStarted();
}

void MediaImpl::startTimer(int duration)
{
    if (duration > 0)
    {
        timer_->start(std::chrono::seconds(duration), [this] { mediaFinished_(); });
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
    stopAttachedMedia();
    onStopped();
}

void MediaImpl::setInTransition(std::unique_ptr<TransitionExecutor>&& transition)
{
    inTransition_ = std::move(transition);
}

void MediaImpl::setOutTransition(std::unique_ptr<TransitionExecutor>&& transition)
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

SignalMediaFinished& MediaImpl::mediaFinished()
{
    return mediaFinished_;
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
