#pragma once

#include "control/media/MediaStat.hpp"

struct LayoutStats
{
    int id = 0;
    DateTime started;
    DateTime finished;
    std::vector<MediaStat> mediaStats;

    void clear()
    {
        *this = LayoutStats{};
    }
};

inline std::ostream& operator<<(std::ostream& out, const LayoutStats& stats)
{
    out << "Layout ID: " << stats.id << " Started: " << stats.started.string()
        << " Finished: " << stats.finished.string() << " Duration: " << (stats.finished - stats.started).seconds()
        << std::endl;
    for (auto&& mediaStat : stats.mediaStats)
    {
        out << mediaStat << std::endl;
    }
    return out;
}
