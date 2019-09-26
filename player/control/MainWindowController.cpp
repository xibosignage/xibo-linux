#include "MainWindowController.hpp"
#include "IMainWindow.hpp"

#include "config.hpp"

#include "control/common/IOverlayLayout.hpp"
#include "control/common/Image.hpp"
#include "control/layout/LayoutsManager.hpp"

namespace ph = std::placeholders;
const std::string StatusScreenKey = "i";

MainWindowController::MainWindowController(const std::shared_ptr<IMainWindow>& window, LayoutsManager& layoutsManager) :
    m_window(window),
    m_layoutsManager(layoutsManager)
{
    m_window->disableWindowDecoration();
    m_window->setCursorVisible(false);
    m_window->keyPressed().connect(std::bind(&MainWindowController::onKeyPressed, this, ph::_1));

    m_layoutsManager.mainLayoutFetched().connect([this](const std::shared_ptr<IOverlayLayout>& layout) {
        if (layout)
        {
            scaleLayout(layout);

            m_window->setMainLayout(layout);
            layout->showAll();
        }
        else
        {
            showSplashScreen();
        }
    });

    m_layoutsManager.overlaysFetched().connect([this](const std::vector<std::shared_ptr<IOverlayLayout>>& overlays) {
        for (auto&& layout : overlays)
        {
            scaleLayout(layout);
            layout->showAll();
        }
        m_window->setOverlays(overlays);
    });
}

void MainWindowController::onKeyPressed(const std::string& pressedKey)
{
    if (pressedKey == StatusScreenKey)
    {
        m_statusScrenRequested();
    }
}

void MainWindowController::showSplashScreen()
{
    auto splashScreen = createSplashScreen();
    m_window->setMainLayout(splashScreen);
    splashScreen->show();
}

StatusScreenRequested& MainWindowController::statusScreenRequested()
{
    return m_statusScrenRequested;
}

std::shared_ptr<IImage> MainWindowController::createSplashScreen()
{
    auto spashImage = std::make_shared<Image>(m_window->width(), m_window->height());

    spashImage->loadFromFile(ProjectResources::splashScreen(), false);

    return spashImage;
}

void MainWindowController::scaleLayout(const std::shared_ptr<IOverlayLayout>& layout)
{
    double scaleX = static_cast<double>(m_window->width()) / layout->width();
    double scaleY = static_cast<double>(m_window->height()) / layout->height();
    double scaleFactor = std::min(scaleX, scaleY);

    layout->scale(scaleFactor, scaleFactor);
}

void MainWindowController::updateWindowDimensions(const PlayerSettings::Dimensions& dimensions)
{
    if (shouldBeFullscreen(dimensions))
    {
        m_window->fullscreen();
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
    if (width != m_window->width() || height != m_window->height())
    {
        m_window->setSize(width, height);
    }
}

// move to MainWindow
void MainWindowController::setWindowPos(int x, int y)
{
    if (x != m_window->x() || y != m_window->y())
    {
        m_window->move(x, y);
    }
}

bool MainWindowController::shouldBeFullscreen(const PlayerSettings::Dimensions& dimensions) const
{
    return dimensions.width == 0 && dimensions.height == 0;
}
