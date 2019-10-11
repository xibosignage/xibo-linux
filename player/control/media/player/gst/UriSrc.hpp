#pragma once

#include "Element.hpp"
#include "common/types/Uri.hpp"

namespace Gst
{
    class UriSrc : public Gst::Element
    {
    public:
        enum class Type
        {
            Local,
            Network
        };

        static Gst::RefPtr<Gst::UriSrc> create(Type type);
        void setLocation(const Uri& uri);

    private:
        UriSrc(std::string_view name, Type type);

    private:
        Type type_;
    };
}
