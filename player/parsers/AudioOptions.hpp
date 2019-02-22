#pragma once

#include "MediaOptions.hpp"

class AudioNodeTag { };

class AudioOptions : public MediaOptions
{
public:
    enum class Mute
    {
        Disable,
        Enable
    };

    enum class Loop
    {
        Disable,
        Enable
    };

    AudioOptions(int id,
                 boost::optional<std::string> path,
                 int duration,
                 boost::optional<Mute> muted,
                 boost::optional<Loop> looped,
                 boost::optional<int> volume);
    AudioOptions(const xml_node& node);
    AudioOptions(const xml_node& node, AudioNodeTag);

    boost::optional<Mute> muted() const;
    boost::optional<Loop> looped() const;
    boost::optional<int> volume() const;

private:
    boost::optional<Mute> m_muted;
    boost::optional<Loop> m_looped;
    boost::optional<int> m_volume;
};

std::istream& operator >>(std::istream& in, AudioOptions::Mute& mute);
std::istream& operator >>(std::istream& in, AudioOptions::Loop& loop);

