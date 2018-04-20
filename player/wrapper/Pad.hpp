#ifndef PAD_HPP
#define PAD_HPP

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
        Pad(GstPad* handler);
        ~Pad();
        PadLinkReturn link(Pad* other);
        GstCaps* get_current_caps() const;

    private:
        GstPad* m_handler;

    };
}


#endif // PAD_HPP
