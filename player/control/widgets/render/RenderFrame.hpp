#pragma once

#include <cairomm/surface.h>

class RenderFrame
{
public:
    virtual ~RenderFrame() = default;
    virtual bool isValid() const = 0;
    // TODO: remove cairomm dependency
    virtual Cairo::RefPtr<Cairo::ImageSurface> surface() = 0;
};
