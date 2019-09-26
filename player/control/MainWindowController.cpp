#include "MainWindowController.hpp"
#include "IMainWindow.hpp"

#include "config.hpp"

#include "control/common/IOverlayLayout.hpp"
#include "control/common/Image.hpp"
#include "control/layout/LayoutsManager.hpp"

namespace ph = std::placeholders;
const std::string StatusScreenKey = "i";

MainWindowController::MainWindowController(const std::shared_ptr<IMainWindow>& window, LayoutsManager& layoutsManager) :
    window_(window),
    layoutsManager_(layoutsManager)
{
    window_->disableWindowDecoration();
    window_->setCursorVisible(false);
    window_->keyPressed().connect(std::bind(&MainWindowController::onKeyPressed, this, ph::_1));

    layoutsManager_.mainLayoutFetched().connect([this](const std::shared_ptr<IOverlayLayout>& layout) {
        if (layout)
        {
            scaleLayout(layout);

            window_->setMainLayout(layout);
            layout->showAll();
        }
        else
        {
            showSplashScreen();
        }
    });

    layoutsManager_.overlaysFetched().connect([this](const std::vector<std::shared_ptr<IOverlayLayout>>& overlays) {
        for (auto&& layout : overlays)
        {
            scaleLayout(layout);
            layout->showAll();
        }
        window_->setOverlays(overlays);
    });
}

void MainWindowController::onKeyPressed(const std::string& pressedKey)
{
    if (pressedKey == StatusScreenKey)
    {
        statusScrenRequested_();
    }
}

void MainWindowController::showSplashScreen()
{
    auto splashScreen = createSplashScreen();
    window_->setMainLayout(splashScreen);
    splashScreen->show();
}

StatusScreenRequested& MainWindowController::statusScreenRequested()
{
    return statusScrenRequested_;
}

std::shared_ptr<IImage> MainWindowController::createSplashScreen()
{
    auto spashImage = std::make_shared<Image>(window_->width(), window_->height());

    spashImage->loadFromFile(ProjectResources::splashScreen(), false);

    return spashImage;
}

void MainWindowController::scaleLayout(const std::shared_ptr<IOverlayLayout>& layout)
{
    double scaleX = static_cast<double>(window_->width()) / layout->width();
    double scaleY = static_cast<double>(window_->height()) / layout->height();
    double scaleFactor = std::min(scaleX, scaleY);

    layout->scale(scaleFactor, scaleFactor);
}

void MainWindowController::updateWindowDimensions(const PlayerSettings::Dimensions& dimensions)
{
    if (shouldBeFullscreen(dimensions))
    {
        window_->fullscreen();
    }
    else
    {
        setWindowSize(dimensions.width, dimensions.height);
        setWindowPos(dimensions.x, dimensions.y);
    }
}

// move to MainWindow
void MainWindowController::setWindowSize(int width, int height)
{
    if (width != window_->width() || height != window_->height())
    {
        window_->setSize(width, height);
    }
}

// move to MainWindow
void MainWindowController::setWindowPos(int x, int y)
{
    if (x != window_->x() || y != window_->y())
    {
        window_->move(x, y);
    }
}

bool MainWindowController::shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const
{
    return dimensions.width == 0 && dimensions.height == 0;
}
