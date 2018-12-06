#pragma once

#include "IMainWindow.hpp"
#include <boost/noncopyable.hpp>

class IMainLayout;

class MainWindow : public IMainWindow, private boost::noncopyable
{
public:
    MainWindow(std::unique_ptr<IWindowAdaptor>&& handler);

    void setSize(int width, int height) override;
    void setPos(int x, int y) override;
    void setKeepAbove(bool keepAbove) override;
    void setFullscreen(bool fullscreen) override;
    void setCursorVisible(bool cursorVisible) override;
    bool isVisible() const override;

    void addLayout(std::unique_ptr<IMainLayout>&& layout) override;
    void showLayout() override;
    IWindowAdaptor& handler() override;

private:
    void scaleLayout(IMainLayout& layout);
    void checkWindowNewSize(int width, int height);
    void checkWindowCoordinates(int x, int y);

private:
    std::unique_ptr<IWindowAdaptor> m_handler;
    std::unique_ptr<IMainLayout> m_layout;

};
