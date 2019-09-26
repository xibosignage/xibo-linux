#pragma once

#include "GstFwd.hpp"

#include <functional>
#include <gst/pbutils/gstdiscoverer.h>

namespace Gst
{
    struct InspectorResult
    {
        int frameWidth = 0;
        int frameHeight = 0;

        bool audioStream = false;
        bool videoStream = false;
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
