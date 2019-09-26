#pragma once

#include "GstFwd.hpp"
#include <gst/gstutils.h>
#include <string>

namespace Gst
{
    enum class State
    {
        VOID_PENDING = GST_STATE_VOID_PENDING,
        NULL_STATE = GST_STATE_NULL,
        READY = GST_STATE_READY,
        PAUSED = GST_STATE_PAUSED,
        PLAYING = GST_STATE_PLAYING
    };

    enum class SeekType
    {
        NONE = GST_SEEK_TYPE_NONE,
        SET = GST_SEEK_TYPE_SET,
        END = GST_SEEK_TYPE_END,
    };

    enum class SeekFlags
    {
        NONE = GST_SEEK_FLAG_NONE,
        FLUSH = GST_SEEK_FLAG_FLUSH,
        ACCURATE = GST_SEEK_FLAG_ACCURATE,
        KEY_UNIT = GST_SEEK_FLAG_KEY_UNIT,
        SEGMENT = GST_SEEK_FLAG_SEGMENT,
        TRICKMODE = GST_SEEK_FLAG_TRICKMODE,
        SKIP = GST_SEEK_FLAG_SKIP,
        SNAP_BEFORE = GST_SEEK_FLAG_SNAP_BEFORE,
        SNAP_AFTER = GST_SEEK_FLAG_SNAP_AFTER,
        SNAP_NEAREST = GST_SEEK_FLAG_SNAP_NEAREST,
        TRICKMODE_KEY_UNITS = GST_SEEK_FLAG_TRICKMODE_KEY_UNITS,
        TRICKMODE_NO_AUDIO = GST_SEEK_FLAG_TRICKMODE_NO_AUDIO
    };

    enum class Format
    {
        UNDEFINED = GST_FORMAT_UNDEFINED,
        DEFAULT = GST_FORMAT_DEFAULT,
        BYTES = GST_FORMAT_BYTES,
        TIME = GST_FORMAT_TIME,
        BUFFERS = GST_FORMAT_BUFFERS,
        PERCENT = GST_FORMAT_PERCENT
    };

    class Element
    {
    public:
        static Gst::RefPtr<Gst::Element> create(std::string_view name);
        virtual ~Element();

        Gst::RefPtr<Gst::Element> link(const Gst::RefPtr<Gst::Element>& other);
        Gst::RefPtr<Gst::Element> linkFltered(const Gst::RefPtr<Gst::Element>& other, GstCaps* filter);

        void setState(Gst::State state);
        Gst::State state() const;
        Gst::RefPtr<Gst::Pad> staticPad(std::string_view name);

        bool seek(gdouble rate, Gst::Format format, Gst::SeekFlags flags, Gst::SeekType startType, gint64 start,
                  Gst::SeekType stopType, gint64 stop);

        GstElement* handler() const;
        void resetHandler();

    protected:
        Element(GstElement* element);
        Element(std::string_view name);

    protected:
        GstElement* element() const;

    private:
        GstElement* m_element = nullptr;
    };
}
