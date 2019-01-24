#pragma once

#include "GstFwd.hpp"

#include <functional>
#include <gst/pbutils/gstdiscoverer.h>

namespace Gst
{
    class InspectorResult
    {
    public:
        unsigned int videoWidth() const;
        unsigned int videoHeigth() const;

        bool hasVideoStream() const;
        bool hasAudioStream() const;

        friend class Inspector;

    private:
        GstDiscovererAudioInfo* m_audioInfo = nullptr;
        GstDiscovererVideoInfo* m_videoInfo = nullptr;

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
        void onFinished(GstDiscoverer*, gpointer);
        void retrieveStreamsInfo(Gst::InspectorResult& result, GList* streams_list);

    private:
        GstDiscoverer* m_discoverer = nullptr;

    };
}
