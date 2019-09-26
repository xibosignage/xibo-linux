#include "Media.hpp"

#include "common/logger/Logging.hpp"
#include "constants.hpp"

Media::Media(const MediaOptions& options, const std::shared_ptr<IWidget>& view) :
    options_(options),
    view_(view),
    timer_(std::make_unique<Timer>())
{
}

void Media::attach(std::unique_ptr<IMedia>&& attachedMedia)
{
    attachedMedia_ = std::move(attachedMedia);
}

void Media::start()
{
    startAttachedMedia();
    startTimer(options_.duration);
    onStarted();
}

void Media::startTimer(int duration)
{
    if (duration > 0)
    {
        timer_->start(std::chrono::seconds(duration), [this] { mediaFinished_(); });
    }
}

void Media::startAttachedMedia()
{
    if (attachedMedia_)
    {
        attachedMedia_->start();
    }
}

void Media::onStarted()
{
    if (view_)
    {
        view_->show();
    }
}

void Media::stop()
{
    stopAttachedMedia();
    onStopped();
}

void Media::setInTransition(std::unique_ptr<TransitionExecutor>&& transition)
{
    inTransition_ = std::move(transition);
}

void Media::setOutTransition(std::unique_ptr<TransitionExecutor>&& transition)
{
    outTransition_ = std::move(transition);
}

void Media::stopAttachedMedia()
{
    if (attachedMedia_)
    {
        attachedMedia_->stop();
    }
}

void Media::applyInTransition()
{
    if (inTransition_)
    {
        inTransition_->apply();
    }
}

void Media::onStopped()
{
    if (view_)
    {
        view_->hide();
    }
}

SignalMediaFinished& Media::mediaFinished()
{
    return mediaFinished_;
}

MediaGeometry::Align Media::align() const
{
    return options_.geometry.align;
}

MediaGeometry::Valign Media::valign() const
{
    return options_.geometry.valign;
}

std::shared_ptr<IWidget> Media::view()
{
    return view_;
}
