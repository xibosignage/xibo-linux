#pragma once

#include <string>
#include <vector>
#include <cairomm/surface.h>

class FilePath;

class ScreenShoter
{
public:
    void take(const FilePath& path);
    std::string takeBase64();

private:
    std::vector<unsigned char> takeScreenshot();
    std::vector<unsigned char> copySurfaceToBuffer(const Cairo::RefPtr<Cairo::Surface>& surface);
    Cairo::RefPtr<Cairo::Surface> takeXDisplayScreenshot();

};
