#pragma once

#include "utils/ITimerProvider.hpp"
#include "control/common/Widget.hpp"
#include "MediaOptions.hpp"

#include <memory>
#include <sigc++/sigc++.h>

using SignalMediaFinished = sigc::signal<void()>;

class Media
{
public:
    Media(const MediaOptions& options, const std::shared_ptr<Widget>& view);
    virtual ~Media() = default;

    void attachMedia(std::unique_ptr<Media>&& attachedMedia);
    void start();
    void stop();

    SignalMediaFinished mediaFinished();
    std::shared_ptr<Widget> view() const;
    MediaGeometry::Align align() const;
    MediaGeometry::Valign valign() const;

protected:
    virtual void onStarted();
    virtual void onStopped();

private:
    void startTimer(int duration);
    void startAttachedMedia();
    void stopAttachedMedia();

private:
    MediaOptions m_options;
    std::shared_ptr<Widget> m_view;
    std::unique_ptr<Media> m_attachedMedia;
    std::unique_ptr<ITimerProvider> m_timer;
    SignalMediaFinished m_mediaFinished;

};
