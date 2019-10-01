#pragma once

#include "control/widgets/render/RenderFrame.hpp"

#include <cairomm/surface.h>
#include <gst/video/video-info.h>
#include <memory>

class GstRenderFrame : public RenderFrame
{
public:
    static std::shared_ptr<GstRenderFrame> create(GstVideoInfo* info, GstBuffer* buffer);
    ~GstRenderFrame() override;

    bool isValid() const override;
    Cairo::RefPtr<Cairo::ImageSurface> surface() override;

private:
    Cairo::RefPtr<Cairo::ImageSurface> createSurface(unsigned char* data, GstVideoInfo* info);
    GstRenderFrame(GstVideoInfo* info, GstBuffer* buffer);

private:
    Cairo::RefPtr<Cairo::ImageSurface> surface_;
    GstVideoFrame frame_;
    bool mapped_ = false;
};
