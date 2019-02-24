#pragma once

#include "MediaOptions.hpp"

class VideoOptions : public MediaOptions
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

    VideoOptions(int id,
                 boost::optional<std::string> path,
                 int duration,
                 boost::optional<Mute> muted,
                 boost::optional<Loop> looped);
    VideoOptions(const xml_node& node);

    boost::optional<Mute> muted() const;
    boost::optional<Loop> looped() const;

private:
    boost::optional<Mute> m_muted;
    boost::optional<Loop> m_looped;
};

std::istream& operator >>(std::istream& in, VideoOptions::Mute& mute);
std::istream& operator >>(std::istream& in, VideoOptions::Loop& loop);
