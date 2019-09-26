#include "XiboVideoSink.hpp"
#include "XiboVideoFrame.hpp"

#include <glibmm/main.h>

static gboolean gst_xibovideosink_set_caps(GstBaseSink* base_sink, GstCaps* caps);
static GstFlowReturn gst_xibovideosink_show_frame(GstVideoSink* video_sink, GstBuffer* buf);
static void gst_xibovideosink_class_init(XiboVideoSinkClass* klass);
static void gst_xibovideosink_init(XiboVideoSink* sink);
static void gst_xibovideosink_finalize(GObject* object);

static const char* string_caps = "video/x-raw, "
                                 "format = (string) { BGRx }, "
                                 "width = (int) [ 1, max ], "
                                 "height = (int) [ 1, max ], "
                                 "framerate = (fraction) [ 0, max ];";
static GstStaticPadTemplate sink_template =
    GST_STATIC_PAD_TEMPLATE("sink", GST_PAD_SINK, GST_PAD_ALWAYS, GST_STATIC_CAPS(string_caps));

G_DEFINE_TYPE(XiboVideoSink, gst_xibovideosink, GST_TYPE_VIDEO_SINK);

static void gst_xibovideosink_class_init(XiboVideoSinkClass* klass)
{
    GObjectClass* g_object_class = G_OBJECT_CLASS(klass);
    GstBaseSinkClass* base_sink_class = GST_BASE_SINK_CLASS(klass);
    GstVideoSinkClass* video_sink_class = GST_VIDEO_SINK_CLASS(klass);

    gst_element_class_add_pad_template(GST_ELEMENT_CLASS(klass), gst_static_pad_template_get(&sink_template));

    gst_element_class_set_static_metadata(GST_ELEMENT_CLASS(klass), "Xibo Video Sink plugin", "Some classification",
                                          "Plugin to provide video playing on drawing area", "Stivius");

    base_sink_class->set_caps = GST_DEBUG_FUNCPTR(gst_xibovideosink_set_caps);
    video_sink_class->show_frame = GST_DEBUG_FUNCPTR(gst_xibovideosink_show_frame);
    g_object_class->finalize = gst_xibovideosink_finalize;
}

void gst_xibovideosink_set_handler(XiboVideoSink* sink, const std::weak_ptr<IVideoWindow>& handler)
{
    sink->handler = handler;
}

static void gst_xibovideosink_init(XiboVideoSink* sink)
{
    gst_video_info_init(&sink->info);
    sink->sinkpad = gst_pad_new_from_static_template(&sink_template, "xibosink");
    gst_element_add_pad(GST_ELEMENT(sink), sink->sinkpad);
}

static void gst_xibovideosink_finalize(GObject* object)
{
    auto sink = GST_XIBOVIDEOSINK(object);

    sink->handler.reset();

    G_OBJECT_CLASS(gst_xibovideosink_parent_class)->finalize(object);
}

static gboolean gst_xibovideosink_set_caps(GstBaseSink* base_sink, GstCaps* caps)
{
    XiboVideoSink* sink = GST_XIBOVIDEOSINK(base_sink);
    gst_video_info_from_caps(&sink->info, caps);
    return true;
}

static GstFlowReturn gst_xibovideosink_show_frame(GstVideoSink* base_sink, GstBuffer* buffer)
{
    XiboVideoSink* sink = GST_XIBOVIDEOSINK(base_sink);
    if (auto frame = XiboVideoFrame::create(&sink->info, buffer))
    {
        Glib::MainContext::get_default()->invoke([handler = sink->handler, frame]() {
            if (auto window = handler.lock())
            {
                window->drawFrame(frame);
            }
            return false;
        });
    }
    return GST_FLOW_OK;
}

gboolean pluginInit(GstPlugin* plugin)
{
    return gst_element_register(plugin, "xibovideosink", 10, GST_TYPE_XIBOVIDEOSINK);
}
