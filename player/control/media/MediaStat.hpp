#pragma once

#include "common/dt/DateTime.hpp"

struct MediaStat
{
    int id = 0;
    DateTime started;
    DateTime finished;

    void clear()
    {
        *this = MediaStat{};
    }
};

inline std::ostream& operator<<(std::ostream& out, const MediaStat& stats)
{
    out << "Media ID: " << stats.id << " Started: " << stats.started.string()
        << " Finished: " << stats.finished.string() << " Duration: " << (stats.finished - stats.started).seconds();
    return out;
}
