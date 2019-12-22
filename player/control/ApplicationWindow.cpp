#include "ApplicationWindow.hpp"

#include "control/widgets/ImageWidgetFactory.hpp"
#include "control/widgets/OverlayLayoutFactory.hpp"
#include "control/widgets/StatusScreenFactory.hpp"
#include "control/widgets/gtk/WindowGtk.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"
#include "config/AppConfig.hpp"

const KeyboardKey StatusScreenKey{"i", 105};
const int MinDisplayWidth = 160;
const int MinDisplayHeight = 120;
const int DefaultPos = 0;

const double StatusScreenScaleX = 0.5;
const double StatusScreenScaleY = 1;

template class ApplicationWindow<WindowGtk>;

template <typename Window>
ApplicationWindow<Window>::ApplicationWindow(int width, int height, int x, int y) :
    layout_(OverlayLayoutFactory::create(width, height)),
    statusScreen_(StatusScreenFactory::create(static_cast<Window&>(*this), MinDisplayWidth, MinDisplayHeight))
{
    assert(layout_);
    assert(statusScreen_);

    Window::setChild(layout_);
    Window::disableWindowDecoration();
    Window::setCursorVisible(false);
    Window::setBackgroundColor(Color::fromString("#000000"));

    Window::setSize(width, height);
    Window::move(x, y);

    Window::keyPressed().connect([this](const KeyboardKey& key) {
        if (key == StatusScreenKey)
        {
            statusScreenRequested_();
            statusScreen_->show();
        }
    });
}

// TODO should we handle exception here?
template <typename Window>
void ApplicationWindow<Window>::setMainLayout(const MainLayoutWidget& child)
{
    assert(child);

    layout_->setMainChild(child);
    scaleLayoutToWindowBounds(child);
}

// TODO should we handle exception here?
template <typename Window>
void ApplicationWindow<Window>::setOverlays(const OverlaysWidgets& children)
{
    layout_->removeChildren();

    for (auto&& child : children)
    {
        // TODO: should be centered?
        layout_->addChild(child, DefaultPos, DefaultPos, DefaultPos);
        scaleLayoutToWindowBounds(child);
    }
}

// TODO should we handle exception here?
template <typename Window>
void ApplicationWindow<Window>::showSplashScreen()
{
    auto splashScreen = createSplashScreen();
    setMainLayout(splashScreen);
    splashScreen->show();
}

template <typename Window>
StatusScreenShown& ApplicationWindow<Window>::statusScreenShown()
{
    return statusScreenRequested_;
}

template <typename Window>
void ApplicationWindow<Window>::updateStatusScreen(const StatusInfo& info)
{
    statusScreen_->setText(info.toString());
}

template <typename Window>
std::shared_ptr<Xibo::Image> ApplicationWindow<Window>::createSplashScreen()
{
    auto spashImage = ImageWidgetFactory::create(Window::width(), Window::height());

    assert(spashImage);

    spashImage->loadFrom(Uri::fromFile(AppConfig::splashScreenPath()), Xibo::Image::PreserveRatio::False);

    return spashImage;
}

template <typename Window>
void ApplicationWindow<Window>::scaleLayoutToWindowBounds(const std::shared_ptr<Xibo::Widget>& layout)
{
    assert(layout);

    double scaleX = static_cast<double>(Window::width()) / layout->width();
    double scaleY = static_cast<double>(Window::height()) / layout->height();
    double scaleFactor = std::min(scaleX, scaleY);

    layout->scale(scaleFactor, scaleFactor);
}

template <typename Window>
void ApplicationWindow<Window>::setSize(int width, int height)
{
    if (shouldBeFullscreen(width, height))
    {
        Window::fullscreen();
    }
    else
    {
        Window::setSize(width, height);
    }

    statusScreen_->setSize(static_cast<int>(this->width() * StatusScreenScaleX),
                           static_cast<int>(this->height() * StatusScreenScaleY));
}

template <typename Window>
void ApplicationWindow<Window>::move(int x, int y)
{
    if (!Window::isFullscreen())
    {
        Window::move(x, y);
    }
}

template <typename Window>
bool ApplicationWindow<Window>::shouldBeFullscreen(int width, int height) const
{
    return width == 0 && height == 0;
}
