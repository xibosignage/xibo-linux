#pragma once

#include "stat/PlayingStat.hpp"

#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>

class StatsRecorder : private boost::noncopyable
{
public:
    struct Record
    {
        enum Type
        {
            Layout,
            Media
        };

        Type type;
        DateTime started;
        DateTime finished;
        int scheduleId;
        int layoutId;
        boost::optional<int> mediaId;
        long duration;
        int count;
    };

    using Records = std::vector<Record>;

    void addLayoutStat(int layoutId, int scheduleId, const PlayingStat& interval);
    void addMediaStat(int scheduleId, int layoutId, int mediaId, const PlayingStat& interval);
    void clear();
    size_t empty() const;
    const Records& records() const;

private:
    Records records_;
};
