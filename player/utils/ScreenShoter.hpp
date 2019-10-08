#pragma once

#include <cairomm/surface.h>
#include <functional>
#include <string>
#include <vector>

#include "control/widgets/gtk/WindowGtk.hpp"

class FilePath;
using ScreenShotTaken = std::function<void(const std::string&)>;
using ImageBufferCreated = std::function<void(const std::vector<unsigned char>&)>;
using SurfaceCreated = std::function<void(const Cairo::RefPtr<Cairo::Surface>&)>;

class ScreenShoter
{
public:
    ScreenShoter(Xibo::Window& window);
    void takeBase64(ScreenShotTaken callback);

private:
    void takeScreenshot(ImageBufferCreated callback);
    std::vector<unsigned char> copySurfaceToBuffer(const Cairo::RefPtr<Cairo::Surface>& surface);
    void takeXDisplayScreenshot(SurfaceCreated callback);

private:
    WindowGtk& window_;
};
