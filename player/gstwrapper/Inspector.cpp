#include "Inspector.hpp"
#include "common/logger/Logging.hpp"

#include <cassert>

namespace ph = std::placeholders;

Gst::RefPtr<Gst::Inspector> Gst::Inspector::create(unsigned timeoutSeconds)
{
    return Gst::RefPtr<Gst::Inspector>(new Gst::Inspector(timeoutSeconds));
}

Gst::Inspector::Inspector(unsigned int timeoutSeconds)
{
    m_discoverer = gst_discoverer_new(timeoutSeconds * GST_SECOND, nullptr);
}

Gst::Inspector::~Inspector()
{
    gst_object_unref(m_discoverer);
}

Gst::InspectorResult Gst::Inspector::processDiscoveredInfo(GstDiscovererInfo* info, GError* err)
{
    Gst::InspectorResult discoveredResult;
    auto uri = gst_discoverer_info_get_uri(info);
    auto result = gst_discoverer_info_get_result(info);

    switch (result)
    {
        case GST_DISCOVERER_URI_INVALID: Log::debug("Invalid URI {}", uri); break;
        case GST_DISCOVERER_ERROR: Log::debug("Discoverer error: {}", err->message); break;
        case GST_DISCOVERER_TIMEOUT: Log::debug("Timeout"); break;
        case GST_DISCOVERER_BUSY: Log::debug("Busy"); break;
        case GST_DISCOVERER_MISSING_PLUGINS:
        {
            auto missingPluginsInfo = gst_discoverer_info_get_missing_elements_installer_details(info);

            Log::debug("Missing plugins");
            for (size_t i = 0; missingPluginsInfo[i]; ++i)
            {
                Log::debug("{}", missingPluginsInfo[i]);
            }
            break;
        }
        case GST_DISCOVERER_OK: Log::debug("Discovered {}", uri); break;
    }

    if (result != GST_DISCOVERER_OK)
    {
        Log::debug("This URI cannot be played");
    }

    auto streamsList = gst_discoverer_info_get_stream_list(info);
    retrieveStreamsInfo(discoveredResult, streamsList);

    if (err)
    {
        g_error_free(err);
    }
    gst_discoverer_stream_info_list_free(streamsList);
    gst_discoverer_info_unref(info);

    return discoveredResult;
}

void Gst::Inspector::retrieveStreamsInfo(Gst::InspectorResult& result, GList* streamsList)
{
    for (auto tmp = streamsList; tmp; tmp = tmp->next)
    {
        auto streamInfo = reinterpret_cast<GstDiscovererStreamInfo*>(tmp->data);

        if (GST_IS_DISCOVERER_VIDEO_INFO(streamInfo))
        {
            auto videoInfo = GST_DISCOVERER_VIDEO_INFO(streamInfo);

            result.videoStream = true;
            result.frameWidth = static_cast<int>(gst_discoverer_video_info_get_width(videoInfo));
            result.frameHeight = static_cast<int>(gst_discoverer_video_info_get_height(videoInfo));
        }

        if (GST_IS_DISCOVERER_AUDIO_INFO(streamInfo))
        {
            result.audioStream = true;
        }
    }
}

Gst::InspectorResult Gst::Inspector::discover(std::string_view uri)
{
    GError* err;
    auto info = gst_discoverer_discover_uri(m_discoverer, uri.data(), &err);

    return processDiscoveredInfo(info, err);
}
