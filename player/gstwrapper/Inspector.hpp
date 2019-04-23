#pragma once

#include "GstFwd.hpp"

#include <functional>
#include <gst/pbutils/gstdiscoverer.h>

namespace Gst
{
    class InspectorResult
    {
    public:
        bool hasVideoStream() const;
        bool hasAudioStream() const;

        friend class Inspector;

    private:
        bool m_audioStream = false;
        bool m_videoStream = false;

    };

    class Inspector
    {
    public:
        static Gst::RefPtr<Gst::Inspector> create(unsigned int timeoutSeconds);
        ~Inspector();

        Gst::InspectorResult discover(std::string_view uri);

    private:
        Inspector(unsigned int timeoutSeconds);

        Gst::InspectorResult processDiscoveredInfo(GstDiscovererInfo* info, GError* err);
        void retrieveStreamsInfo(Gst::InspectorResult& result, GList* streams_list);

    private:
        GstDiscoverer* m_discoverer = nullptr;

    };
}
