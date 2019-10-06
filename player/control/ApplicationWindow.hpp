#pragma once

#include "control/StatusInfo.hpp"
#include "control/widgets/Image.hpp"
#include "control/widgets/OverlayLayout.hpp"
#include "control/widgets/StatusScreen.hpp"
#include "control/widgets/Window.hpp"

using MainLayoutWidget = std::shared_ptr<Xibo::Widget>;
using OverlaysWidgets = std::vector<std::shared_ptr<Xibo::Widget>>;
using StatusScreenShown = boost::signals2::signal<void()>;

template <typename Window>
class ApplicationWindow : public Window
{
public:
    ApplicationWindow();

    void setMainLayout(const MainLayoutWidget& child);
    void setOverlays(const OverlaysWidgets& children);
    void showSplashScreen();

    StatusScreenShown& statusScreenShown();
    void updateStatusScreen(const StatusInfo& info);

    void setSize(int width, int height);
    void move(int x, int y);

private:
    bool shouldBeFullscreen(int width, int height) const;
    std::shared_ptr<Xibo::Image> createSplashScreen();
    void scaleLayoutToWindowBounds(const std::shared_ptr<Xibo::Widget>& layout);

protected:
    std::shared_ptr<Xibo::OverlayLayout> layout_;
    std::shared_ptr<Xibo::StatusScreen> statusScreen_;
    StatusScreenShown statusScreenRequested_;
};
