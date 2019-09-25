#include "XiboVideoFrame.hpp"

std::shared_ptr<XiboVideoFrame> XiboVideoFrame::create(GstVideoInfo* info, GstBuffer* buffer)
{
    auto frame = std::shared_ptr<XiboVideoFrame>(new XiboVideoFrame{info, buffer});
    if (frame->isValid())
    {
        return frame;
    }
    return nullptr;
}

XiboVideoFrame::~XiboVideoFrame()
{
    if (m_mapped)
    {
        gst_video_frame_unmap(&m_frame);
    }
}

bool XiboVideoFrame::isValid() const
{
    return m_mapped && m_surface;
}

Cairo::RefPtr<Cairo::ImageSurface> XiboVideoFrame::surface()
{
    return m_surface;
}

Cairo::RefPtr<Cairo::ImageSurface> XiboVideoFrame::createSurface(unsigned char* data, GstVideoInfo* info)
{
    int width = GST_VIDEO_INFO_WIDTH(info);
    int height = GST_VIDEO_INFO_HEIGHT(info);
    int planeStride = GST_VIDEO_INFO_PLANE_STRIDE(info, 0);

    return Cairo::ImageSurface::create(data, Cairo::FORMAT_RGB24, width, height, planeStride);
}

XiboVideoFrame::XiboVideoFrame(GstVideoInfo* info, GstBuffer* buffer)
{
    m_mapped = gst_video_frame_map(&m_frame, info, buffer, GST_MAP_READ);
    if (m_mapped)
    {
        auto data = static_cast<unsigned char*>(m_frame.data[0]);
        m_surface = createSurface(data, info);
    }
}
