#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
    class VideoOptions : public MediaOptions
    {
    public:
        VideoOptions(const xlf_node& node);

        boost::optional<bool> muted() const;
        boost::optional<bool> looped() const;

    private:
        boost::optional<bool> m_muted;
        boost::optional<bool> m_looped;
    };
}
