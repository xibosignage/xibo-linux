#pragma once

#include "control/status/StatusInfo.hpp"
#include "control/widgets/Image.hpp"
#include "control/widgets/OverlayContainer.hpp"
#include "control/widgets/StatusScreenFactory.hpp"
#include "control/widgets/Window.hpp"

#include "common/logger/Logging.hpp"
#include "common/types/Uri.hpp"
#include "config/AppConfig.hpp"

using MainLayoutWidget = std::shared_ptr<Xibo::Widget>;
using OverlaysWidgets = std::vector<std::shared_ptr<Xibo::Widget>>;
using StatusScreenShown = boost::signals2::signal<void()>;

template <typename Window>
class ApplicationWindow : public Window
{
    static constexpr const double StatusScreenScaleX = 0.5;
    static constexpr const double StatusScreenScaleY = 0.9;
    static constexpr const int MinStatusScreenWidth = 160;
    static constexpr const int MinStatusScreenHeight = 120;
    static constexpr const int DefaultPos = 0;
    static inline const KeyboardKey StatusScreenKey{"i", 105};

public:
    static std::unique_ptr<ApplicationWindow<Window>> create(int width, int height, int x, int y)
    {
        namespace ph = std::placeholders;

        std::unique_ptr<ApplicationWindow<Window>> window(new ApplicationWindow<Window>());

        window->setSize(width, height);
        window->move(x, y);
        window->setMainContainer(OverlayContainerFactory::create(window->width(), window->height()));
        window->disableWindowDecoration();
        window->setCursorVisible(false);
        window->setBackgroundColor(Color::fromString("#000000"));
        window->keyPressed().connect(std::bind(&ApplicationWindow::onKeyPressed, window.get(), ph::_1));

        return window;
    }

    // TODO should we handle exception here?
    void setMainLayout(const MainLayoutWidget& child)
    {
        assert(child);

        scaleLayout(child);
        container_->setMainChild(child);
    }

    // TODO should we handle exception here?
    void setOverlays(const OverlaysWidgets& children)
    {
        container_->removeOverlays();

        for (auto&& child : children)
        {
            scaleLayout(child);
            // TODO: should be centered?
            container_->add(child, DefaultPos, DefaultPos, DefaultPos);
        }
    }

    void showSplashScreen()
    {
        try
        {
            auto splashScreen = createSplashScreen();
            setMainLayout(splashScreen);
            splashScreen->show();
        }
        catch (PlayerRuntimeError& e)
        {
            Log::error("[SplashScreen] {}", e.message());
        }
        catch (std::exception& e)
        {
            Log::error("[SplashScreen] {}", e.what());
        }
    }

    StatusScreenShown& statusScreenShown()
    {
        return statusScreenShown_;
    }

    ExitWithoutRestartRequested& exitWithoutRestartRequested()
    {
        return statusScreen_->exitWithoutRestartRequested();
    }

    void updateStatusScreen(const StatusInfo& info, const std::vector<std::string>& files)
    {
        statusScreen_->setMainInfo(info.toString());
        statusScreen_->setInvalidFiles(files);
    }

    void setSize(int width, int height) override
    {
        auto [adjustedWidth, adjustedHeight] = adjustSize(width, height);

        if (shouldBeFullscreen(adjustedWidth, adjustedHeight))
        {
            Window::fullscreen();
        }
        else
        {
            if (Window::isFullscreen())
            {
                Window::unfullscreen();
            }
            Window::setSize(adjustedWidth, adjustedHeight);
        }

        statusScreen_->setSize(static_cast<int>(this->width() * StatusScreenScaleX),
                               static_cast<int>(this->height() * StatusScreenScaleY));
    }

private:
    ApplicationWindow() :
        statusScreen_(
            StatusScreenFactory::create(static_cast<Window&>(*this), MinStatusScreenWidth, MinStatusScreenHeight))
    {
        assert(statusScreen_);
    }

    std::pair<int, int> adjustSize(int width, int height)
    {
        if (width < 0 || height < 0)
        {
            Log::error("[ApplicationWindow] Negative size is not allowed, falling back to fullscreen mode");
            return {0, 0};
        }
        return {width, height};
    }

    void setMainContainer(const std::shared_ptr<Xibo::OverlayContainer>& widget)
    {
        assert(widget);

        this->add(widget);
        container_ = widget;
    }

    void scaleLayout(const std::shared_ptr<Xibo::Widget>& layout)
    {
        auto scaleFactor = this->calcScaleFactor(layout->width(), layout->height(), this->width(), this->height());
        layout->scale(scaleFactor, scaleFactor);
    }

    void onKeyPressed(const KeyboardKey& key)
    {
        if (key == StatusScreenKey)
        {
            statusScreenShown_();
            statusScreen_->showAll();
        }
    }

    bool shouldBeFullscreen(int width, int height) const
    {
        return width == 0 && height == 0;
    }

    std::shared_ptr<Xibo::Image> createSplashScreen()
    {
        return ImageWidgetFactory::create(Uri::fromFile(AppConfig::splashScreenPath()),
                                          this->width(),
                                          this->height(),
                                          Xibo::Image::PreserveRatio::False);
    }

private:
    std::shared_ptr<Xibo::OverlayContainer> container_;
    std::shared_ptr<Xibo::StatusScreen> statusScreen_;
    StatusScreenShown statusScreenShown_;
};
