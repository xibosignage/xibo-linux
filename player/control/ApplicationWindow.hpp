#pragma once

#include "control/StatusScreenFormatter.hpp"
#include "control/widgets/OverlayLayout.hpp"
#include "control/widgets/StatusScreen.hpp"
#include "control/widgets/Window.hpp"

using MainLayoutWidget = std::shared_ptr<Xibo::Widget>;
using OverlaysWidgets = std::vector<std::shared_ptr<Xibo::Widget>>;
using StatusScreenRequested = boost::signals2::signal<void()>;

namespace Xibo
{
    class Image;
}

template <typename Window>
class ApplicationWindow : public Window
{
public:
    ApplicationWindow();

    void setMainLayout(const MainLayoutWidget& child);
    void setOverlays(const OverlaysWidgets& children);
    void showSplashScreen();

    StatusScreenRequested& statusScreenRequested();
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
    StatusScreenRequested statusScreenRequested_;
};
