#pragma once

#include "MainWindow.hpp"

#include "managers/PlayerSettings.hpp"
#include "control/layout/MainLayout.hpp"

class XiboLayoutScheduler;

using StatusScreenRequested = sigc::signal<void()>;

class MainWindowController
{
public:
    MainWindowController(const std::shared_ptr<MainWindow>& window, XiboLayoutScheduler& scheduler);

    void updateLayout(int layoutId);
    void updateWindowDimensions(const PlayerSettings::Dimensions& dimensions);
    void showSplashScreen();

    StatusScreenRequested statusScreenRequested();

private:
    void showLayout(int layoutId);
    std::unique_ptr<MainLayout> createLayout(int layoutId);
    std::shared_ptr<Widget> createSplashScreen();
    void scaleLayout(const std::shared_ptr<MainLayoutView>& layout);
    void setWindowSize(int width, int height);
    void setWindowPos(int x, int y);
    bool shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const;
    void onKeyPressed(const std::string& pressedKey);

private:
    std::shared_ptr<MainWindow> m_window;
    XiboLayoutScheduler& m_scheduler;
    StatusScreenRequested m_statusScrenRequested;
    std::unique_ptr<MainLayout> m_layout;

};
