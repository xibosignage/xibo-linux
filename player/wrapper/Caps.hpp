#pragma once

#include "GstFwd.hpp"
#include <gst/gstcaps.h>
#include <string>

namespace Gst
{
    class Structure
    {
    public:
        Structure(GstStructure* handler);
        int getHeight() const;
        int getWidth() const;

    private:
        GstStructure* m_handler = nullptr;

    };

    class Caps
    {
    public:
        Caps(GstCaps* getHandler);
        Caps(const std::string& stringCaps);
        Caps(Caps&& other);
        Caps& operator=(Caps&& other);
        ~Caps();
        static Gst::RefPtr<Gst::Caps> create(const std::string& stringCaps);
        Gst::RefPtr<Gst::Structure> getStructure(guint index) const;
        GstCaps* getHandler();

//    private:
        std::string m_stringCaps;
        GstCaps* m_handler = nullptr;
    };
}
