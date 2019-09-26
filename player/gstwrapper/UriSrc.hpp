#pragma once

#include "Element.hpp"
#include "common/uri/Uri.hpp"

namespace Gst
{
    class UriSrc : public Gst::Element
    {
    public:
        static Gst::RefPtr<Gst::UriSrc> create(Uri::Scheme scheme);
        void setLocation(const Uri& uri);

    private:
        UriSrc(std::string_view name, Uri::Scheme scheme);

    private:
        Uri::Scheme m_fileType;
    };
}
