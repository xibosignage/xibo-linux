#ifndef PAD_HPP
#define PAD_HPP

#include <gst/gstpad.h>
#include <string>
#include <memory>

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

    template<typename T>
    using RefPtr = std::shared_ptr<T>;

    class Caps;

    class Pad
    {
    public:
        Pad(GstPad* handler, bool take_ownership = true);
        ~Pad();
        Gst::PadLinkReturn link(const Gst::RefPtr<Gst::Pad>& other);
        Gst::RefPtr<Gst::Caps> get_current_caps() const;

    private:
        GstPad* m_handler;
        bool m_take_ownership;

    };
}


#endif // PAD_HPP
