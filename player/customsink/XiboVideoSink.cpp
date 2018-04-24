#include "XiboVideoSink.hpp"

#include <spdlog/spdlog.h>
#include <boost/format.hpp>

static gboolean gst_xibovideosink_set_caps(GstBaseSink* base_sink, GstCaps* caps);
static GstFlowReturn gst_xibovideosink_show_frame(GstVideoSink* video_sink, GstBuffer* buf);
static bool gst_xibovideosink_on_frame_drawn(XiboVideoSink* sink, const Cairo::RefPtr<::Cairo::Context>& cairo);
static void gst_xibovideosink_class_init(XiboVideoSinkClass* klass);
static void gst_xibovideosink_init(XiboVideoSink* sink);
static GstStaticPadTemplate sink_template;
static std::string string_caps;

G_DEFINE_TYPE(XiboVideoSink, gst_xibovideosink, GST_TYPE_VIDEO_SINK);

static void gst_xibovideosink_class_init(XiboVideoSinkClass* klass)
{
    GstBaseSinkClass* base_sink_class = GST_BASE_SINK_CLASS(klass);
    GstVideoSinkClass* video_sink_class = GST_VIDEO_SINK_CLASS(klass);

    gst_element_class_add_pad_template(GST_ELEMENT_CLASS(klass),
            gst_static_pad_template_get(&sink_template));

    gst_element_class_set_static_metadata( GST_ELEMENT_CLASS(klass),
        "Xibo Video Sink plugin",
        "Some classification",
        "Plugin to provide video playing on drawing area",
        "Stivius"
    );

    base_sink_class->set_caps = GST_DEBUG_FUNCPTR(gst_xibovideosink_set_caps);
    video_sink_class->show_frame = GST_DEBUG_FUNCPTR(gst_xibovideosink_show_frame);
}

void gst_xibovideosink_set_handler(XiboVideoSink* sink, Gtk::DrawingArea* handler)
{
    sink->handler = handler;    
    sink->handler->signal_draw().connect(sigc::bind<0>(sigc::ptr_fun(gst_xibovideosink_on_frame_drawn), sink));
}

static void gst_xibovideosink_init(XiboVideoSink* sink)
{
    gst_video_info_init(&sink->info);
    sink->sinkpad = gst_pad_new_from_static_template(&sink_template, "xibosink");
    gst_element_add_pad (GST_ELEMENT(sink), sink->sinkpad);
}

static gboolean gst_xibovideosink_set_caps(GstBaseSink* sink, GstCaps* caps)
{
    XiboVideoSink* video_sink = GST_XIBOVIDEOSINK(sink);
    gst_video_info_from_caps(&video_sink->info, caps);
    return true;
}

static GstFlowReturn gst_xibovideosink_show_frame(GstVideoSink* sink, GstBuffer* buf)
{
    XiboVideoSink* video_sink = GST_XIBOVIDEOSINK(sink);
    GstVideoFrame frame;
    if(gst_video_frame_map(&frame, &video_sink->info, buf, GST_MAP_READ))
    {
        video_sink->surface = Cairo::ImageSurface::create(static_cast<u_char*>(frame.data[0]),
                                                          Cairo::FORMAT_RGB24,
                                                          GST_VIDEO_INFO_WIDTH(&video_sink->info),
                                                          GST_VIDEO_INFO_HEIGHT(&video_sink->info),
                                                          GST_VIDEO_INFO_PLANE_STRIDE(&video_sink->info, 0));
        video_sink->handler->queue_draw();
        gst_video_frame_unmap(&frame);
    }
    return GST_FLOW_OK;
}

bool gst_xibovideosink_on_frame_drawn(XiboVideoSink* sink, const Cairo::RefPtr<::Cairo::Context>& cairo)
{
    if(sink->surface)
    {
        cairo->set_source(sink->surface, 0, 0);
        cairo->paint();
    }
    return true;
}

gboolean plugin_init(GstPlugin* plugin)
{
    return gst_element_register(plugin, "xibovideosink", 10, GST_TYPE_XIBOVIDEOSINK);
}

void gst_static_pads_init(int width, int height)
{
    string_caps = (boost::format("video/x-raw, "
                                 "format = (string) { BGRx }, "
                                 "width = (int) %1%, "
                                 "height = (int) %2%, "
                                 "framerate = (fraction) [ 0, max ];") % width % height).str();

    sink_template = GST_STATIC_PAD_TEMPLATE(
        "sink",
        GST_PAD_SINK,
        GST_PAD_ALWAYS,
        GST_STATIC_CAPS(string_caps.c_str())
    );
}
