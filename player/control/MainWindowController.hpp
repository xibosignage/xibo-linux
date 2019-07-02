#pragma once

#include "managers/PlayerSettings.hpp"
#include "control/layout/IMainLayout.hpp"

#include <memory>
#include <sigc++/signal.h>

class XiboLayoutScheduler;
class IMainWindow;
class IMainLayoutView;
class IImage;

using StatusScreenRequested = sigc::signal<void()>;

class MainWindowController
{
public:
    MainWindowController(const std::shared_ptr<IMainWindow>& window, XiboLayoutScheduler& scheduler);

    void updateLayout(int layoutId);
    void updateWindowDimensions(const PlayerSettings::Dimensions& dimensions);
    void showSplashScreen();

    StatusScreenRequested statusScreenRequested();

private:
    void showLayout(int layoutId);
    std::unique_ptr<IMainLayout> createLayout(int layoutId);
    std::shared_ptr<IImage> createSplashScreen();
    void scaleLayout(const std::shared_ptr<IMainLayoutView>& layout);
    void setWindowSize(int width, int height);
    void setWindowPos(int x, int y);
    bool shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const;
    void onKeyPressed(const std::string& pressedKey);

private:
    std::shared_ptr<IMainWindow> m_window;
    XiboLayoutScheduler& m_scheduler;
    StatusScreenRequested m_statusScrenRequested;
    std::unique_ptr<IMainLayout> m_layout;

};
