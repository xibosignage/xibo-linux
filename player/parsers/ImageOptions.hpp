#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
class ImageOptions : public MediaOptions
{
    public:
        ImageOptions(const xlf_node& node);

        boost::optional<std::string> scaleType() const;
        boost::optional<std::string> align() const;
        boost::optional<std::string> valign() const;

    private:
        boost::optional<std::string> m_scaleType;
        boost::optional<std::string> m_align;
        boost::optional<std::string> m_valign;
    };
}
