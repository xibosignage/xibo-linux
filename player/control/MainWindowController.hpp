#pragma once

#include "MainWindow.hpp"
#include "StatusScreenController.hpp"

#include "managers/PlayerSettings.hpp"
#include "control/layout/MainLayout.hpp"

class LayoutScheduler;
class MainLayoutView;

class MainWindowController
{
public:
    MainWindowController(std::shared_ptr<MainWindow> window, LayoutScheduler& scheduler);

    void updateLayout(int layoutId);
    void updateWindowDimensions(const PlayerSettings::Dimensions& dimensions);
    void showSplashScreen();

private:
    void showLayout(int layoutId);
    std::unique_ptr<MainLayout> createLayout(int layoutId);
    std::shared_ptr<Widget> createSplashScreen();
    void scaleLayout(const std::shared_ptr<MainLayoutView>& layout);
    void setWindowSize(int width, int height);
    void setWindowPos(int x, int y);
    bool shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const;

private:
    std::shared_ptr<MainWindow> m_window;
    LayoutScheduler& m_scheduler;
    std::unique_ptr<StatusScreenController> m_statusScreenController;
    std::unique_ptr<MainLayout> m_layout;

};
