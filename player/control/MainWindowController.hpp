#pragma once

#include "managers/PlayerSettings.hpp"

#include <memory>
#include <sigc++/signal.h>

class IMainWindow;
class IOverlayLayout;
class IImage;
class LayoutsManager;

using StatusScreenRequested = sigc::signal<void()>;

class MainWindowController
{
public:
    MainWindowController(const std::shared_ptr<IMainWindow>& window, LayoutsManager& layoutsManager);

    void updateWindowDimensions(const PlayerSettings::Dimensions& dimensions);
    void showSplashScreen();

    StatusScreenRequested statusScreenRequested();

private:
    std::shared_ptr<IImage> createSplashScreen();  
    void scaleLayout(const std::shared_ptr<IOverlayLayout>& layout);

    void setWindowSize(int width, int height);
    void setWindowPos(int x, int y);
    bool shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const;
    void onKeyPressed(const std::string& pressedKey);

private:
    std::shared_ptr<IMainWindow> m_window;
    LayoutsManager& m_layoutsManager;
    StatusScreenRequested m_statusScrenRequested;

};
