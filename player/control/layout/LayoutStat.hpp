#pragma once

#include "common/dt/DateTime.hpp"

struct LayoutStat
{
    int id = 0;
    DateTime started;
    DateTime finished;

    void clear()
    {
        *this = LayoutStat{};
    }
};

inline std::ostream& operator<<(std::ostream& out, const LayoutStat& stats)
{
    out << "Layout ID: " << stats.id << " Started: " << stats.started.string()
        << " Finished: " << stats.finished.string() << " Duration: " << (stats.finished - stats.started).seconds()
        << std::endl;
    return out;
}
