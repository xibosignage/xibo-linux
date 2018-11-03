#pragma once

#include "GstFwd.hpp"
#include <gst/gstpad.h>
#include <string>

namespace Gst
{
    enum class PadLinkReturn
    {
      OK = GST_PAD_LINK_OK,
      WRONG_HIERARCHY = GST_PAD_LINK_WRONG_HIERARCHY,
      WAS_LINKED = GST_PAD_LINK_WAS_LINKED,
      WRONG_DIRECTION = GST_PAD_LINK_WRONG_DIRECTION,
      NOFORMAT = GST_PAD_LINK_NOFORMAT,
      NOSCHED = GST_PAD_LINK_NOSCHED,
      REFUSED = GST_PAD_LINK_REFUSED
    };

    class Pad
    {
    public:
        Pad(GstPad* handler, bool takeOwnership = true);
        ~Pad();
        Gst::PadLinkReturn link(const Gst::RefPtr<Gst::Pad>& other);
        Gst::RefPtr<Gst::Caps> getCurrentCaps() const;

    private:
        GstPad* m_handler;
        bool m_takeOwnership;

    };
}
