#pragma once

#include "control/common/IWidget.hpp"

#include <boost/signals2/signal.hpp>
#include <memory>
#include <vector>

using SignalKeyPressed = boost::signals2::signal<void(std::string)>;
class IOverlayLayout;

class IMainWindow : public IWidget
{
public:
    virtual int x() const = 0;
    virtual int y() const = 0;
    virtual void move(int x, int y) = 0;

    virtual void setMainLayout(const std::shared_ptr<IWidget>& child) = 0;
    virtual void setOverlays(const std::vector<std::shared_ptr<IOverlayLayout>>& children) = 0;
    virtual void disableWindowResize() = 0;
    virtual void disableWindowDecoration() = 0;
    virtual void setKeepAbove(bool keepAbove) = 0;
    virtual void fullscreen() = 0;
    virtual void unfullscreen() = 0;
    virtual void setCursorVisible(bool cursorVisible) = 0;

    virtual SignalKeyPressed& keyPressed() = 0;
};
