#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
    class VideoOptions : public MediaOptions
    {
    public:
        VideoOptions(int id,
                     boost::optional<std::string> path,
                     int duration,
                     boost::optional<bool> muted,
                     boost::optional<bool> looped);
        VideoOptions(const xml_node& node);

        boost::optional<bool> muted() const;
        boost::optional<bool> looped() const;

    private:
        boost::optional<bool> m_muted;
        boost::optional<bool> m_looped;
    };
}
