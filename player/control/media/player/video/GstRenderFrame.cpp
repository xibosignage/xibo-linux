#include "GstRenderFrame.hpp"

std::shared_ptr<GstRenderFrame> GstRenderFrame::create(GstVideoInfo* info, GstBuffer* buffer)
{
    auto frame = std::shared_ptr<GstRenderFrame>(new GstRenderFrame{info, buffer});
    if (frame->isValid())
    {
        return frame;
    }
    return nullptr;
}

GstRenderFrame::~GstRenderFrame()
{
    if (mapped_)
    {
        gst_video_frame_unmap(&frame_);
    }
}

bool GstRenderFrame::isValid() const
{
    return mapped_ && surface_;
}

Cairo::RefPtr<Cairo::ImageSurface> GstRenderFrame::surface()
{
    return surface_;
}

Cairo::RefPtr<Cairo::ImageSurface> GstRenderFrame::createSurface(unsigned char* data, GstVideoInfo* info)
{
    int width = GST_VIDEO_INFO_WIDTH(info);
    int height = GST_VIDEO_INFO_HEIGHT(info);
    int planeStride = GST_VIDEO_INFO_PLANE_STRIDE(info, 0);

    return Cairo::ImageSurface::create(data, Cairo::FORMAT_RGB24, width, height, planeStride);
}

GstRenderFrame::GstRenderFrame(GstVideoInfo* info, GstBuffer* buffer)
{
    mapped_ = gst_video_frame_map(&frame_, info, buffer, GST_MAP_READ);
    if (mapped_)
    {
        auto data = static_cast<unsigned char*>(frame_.data[0]);
        surface_ = createSurface(data, info);
    }
}
