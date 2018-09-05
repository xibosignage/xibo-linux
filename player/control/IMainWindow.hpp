#pragma once

#include <memory>

class IMainLayout;
class IWindowAdaptor;

class IMainWindow
{
public:
    virtual ~IMainWindow() = default;

    virtual void setPos(int x, int y) = 0;
    virtual void setKeepAbove(bool keepAbove) = 0;
    virtual void setFullscreen(bool fullscreen) = 0;
    virtual void setCursorVisible(bool cursorVisible) = 0;
    virtual bool isVisible() const = 0;

    virtual void addLayout(std::unique_ptr<IMainLayout> layout) = 0;
    virtual void showLayout() = 0;
    virtual IWindowAdaptor& handler() = 0;
};
