#ifndef ELEMENT_HPP
#define ELEMENT_HPP

#include <gst/gstutils.h>
#include <string>

#include "Pad.hpp"

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
      TRICMODE = GST_SEEK_FLAG_TRICKMODE,
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
        virtual ~Element();
        Gst::Element* link(Gst::Element* other);
        void set_state(Gst::State state);
        Gst::Pad* get_static_pad(const std::string& name);
        static Gst::Element* create(const std::string& name);
        bool seek(gdouble rate, Gst::Format format, Gst::SeekFlags flags,
                  Gst::SeekType start_type, gint64 start, Gst::SeekType stop_type, gint64 stop);
        GstElement* get_handler() const;

    protected:
        Element();
        Element(const std::string& name);

    protected:
        GstElement* m_element;

    };
}

#endif // ELEMENT_HPP
