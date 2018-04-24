#ifndef CAPS_HPP
#define CAPS_HPP

#include <gst/gstcaps.h>
#include <string>
#include <memory>

namespace Gst
{
    template<typename T>
    using RefPtr = std::shared_ptr<T>;

    class Structure
    {
    public:
        Structure(GstStructure* handler);
        int get_height() const;
        int get_width() const;

    private:
        GstStructure* m_handler;

    };

    class Caps
    {
    public:
        Caps(GstCaps* handler);
        ~Caps();
        Gst::RefPtr<Gst::Structure> get_structure(guint index) const;

//    private:
        GstCaps* m_handler;

    };
}



#endif // CAPS_HPP
