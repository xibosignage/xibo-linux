#pragma once

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
        GstStructure* m_handler = nullptr;

    };

    class Caps
    {
    public:
        Caps(GstCaps* get_handler);
        Caps(const std::string& string_caps);
        Caps(Caps&& other);
        Caps& operator=(Caps&& other);
        ~Caps();
        static Gst::RefPtr<Gst::Caps> create(const std::string& string_caps);
        Gst::RefPtr<Gst::Structure> get_structure(guint index) const;
        GstCaps* get_handler();

    private:
        std::string m_string_caps;
        GstCaps* m_handler = nullptr;
    };
}
