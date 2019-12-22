#pragma once

#include "control/StatusInfo.hpp"
#include "control/widgets/ImageWidgetFactory.hpp"
#include "control/widgets/OverlayLayoutFactory.hpp"
#include "control/widgets/StatusScreenFactory.hpp"
#include "control/widgets/Window.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"
#include "config/AppConfig.hpp"

const KeyboardKey StatusScreenKey{"i", 105};
const int MinDisplayWidth = 160;
const int MinDisplayHeight = 120;
const int DefaultPos = 0;

const double StatusScreenScaleX = 0.5;
const double StatusScreenScaleY = 1;

using MainLayoutWidget = std::shared_ptr<Xibo::Widget>;
using OverlaysWidgets = std::vector<std::shared_ptr<Xibo::Widget>>;
using StatusScreenShown = boost::signals2::signal<void()>;

template <typename Window>
class ApplicationWindow : public Window
{
public:
    ApplicationWindow() :
        layout_(OverlayLayoutFactory::create(MinDisplayWidth, MinDisplayHeight)),
        statusScreen_(StatusScreenFactory::create(static_cast<Window&>(*this), MinDisplayWidth, MinDisplayHeight))
    {
        assert(layout_);
        assert(statusScreen_);

        Window::setChild(layout_);
        Window::disableWindowDecoration();
        Window::setCursorVisible(false);
        Window::setBackgroundColor(Color::fromString("#000000"));
        Window::keyPressed().connect([this](const KeyboardKey& key) {
            if (key == StatusScreenKey)
            {
                statusScreenRequested_();
                statusScreen_->show();
            }
        });
    }

    // TODO should we handle exception here?
    void setMainLayout(const MainLayoutWidget& child)
    {
        assert(child);

        layout_->setMainChild(child);
        scaleLayoutToWindowBounds(child);
    }

    // TODO should we handle exception here?
    void setOverlays(const OverlaysWidgets& children)
    {
        layout_->removeChildren();

        for (auto&& child : children)
        {
            // TODO: should be centered?
            layout_->addChild(child, DefaultPos, DefaultPos, DefaultPos);
            scaleLayoutToWindowBounds(child);
        }
    }
    void showSplashScreen()
    {
        auto splashScreen = createSplashScreen();
        setMainLayout(splashScreen);
        splashScreen->show();
    }

    StatusScreenShown& statusScreenShown()
    {
        return statusScreenRequested_;
    }

    void updateStatusScreen(const StatusInfo& info)
    {
        statusScreen_->setText(info.toString());
    }

    void setSize(int width, int height) override
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

    void move(int x, int y) override
    {
        if (!Window::isFullscreen())
        {
            Window::move(x, y);
        }
    }

private:
    bool shouldBeFullscreen(int width, int height) const
    {
        return width == 0 && height == 0;
    }

    // HANDLE exception here
    std::shared_ptr<Xibo::Image> createSplashScreen()
    {
        auto spashImage = ImageWidgetFactory::create(Window::width(), Window::height());

        assert(spashImage);

        spashImage->loadFrom(Uri::fromFile(AppConfig::splashScreenPath()), Xibo::Image::PreserveRatio::False);

        return spashImage;
    }

    void scaleLayoutToWindowBounds(const std::shared_ptr<Xibo::Widget>& layout)
    {
        assert(layout);

        double scaleX = static_cast<double>(Window::width()) / layout->width();
        double scaleY = static_cast<double>(Window::height()) / layout->height();
        double scaleFactor = std::min(scaleX, scaleY);

        layout->scale(scaleFactor, scaleFactor);
    }

protected:
    std::shared_ptr<Xibo::OverlayLayout> layout_;
    std::shared_ptr<Xibo::StatusScreen> statusScreen_;
    StatusScreenShown statusScreenRequested_;
};
