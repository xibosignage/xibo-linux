#pragma once

#include "MediaOptions.hpp"

namespace ResourcesXlf
{
    class AudioNodeTag { };

    class AudioOptions : public MediaOptions
    {
    public:
        AudioOptions(const xlf_node& node);
        AudioOptions(const xlf_node& node, AudioNodeTag);

        boost::optional<bool> muted() const;
        boost::optional<bool> looped() const;
        boost::optional<int> volume() const;

    private:
        boost::optional<bool> m_muted;
        boost::optional<bool> m_looped;
        boost::optional<int> m_volume;
    };
}
