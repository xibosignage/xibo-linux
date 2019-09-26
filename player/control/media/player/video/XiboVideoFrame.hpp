#pragma once

#include <gst/video/video-info.h>

#include <cairomm/surface.h>
#include <memory>

class XiboVideoFrame
{
public:
    static std::shared_ptr<XiboVideoFrame> create(GstVideoInfo* info, GstBuffer* buffer);
    ~XiboVideoFrame();

    bool isValid() const;
    Cairo::RefPtr<Cairo::ImageSurface> surface();

private:
    Cairo::RefPtr<Cairo::ImageSurface> createSurface(unsigned char* data, GstVideoInfo* info);
    XiboVideoFrame(GstVideoInfo* info, GstBuffer* buffer);

private:
    Cairo::RefPtr<Cairo::ImageSurface> m_surface;
    GstVideoFrame m_frame;
    bool m_mapped = false;
};
