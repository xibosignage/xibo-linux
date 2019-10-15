#pragma once

#include "common/dt/DateTime.hpp"

#include "control/layout/LayoutStat.hpp"
#include "control/media/MediaStat.hpp"

#include <boost/noncopyable.hpp>

class StatsRecorder : private boost::noncopyable
{
public:
    struct Record
    {
        std::string type;
        DateTime started;
        DateTime finished;
        int scheduleId;
        int layoutId;
        int mediaId;
        long duration;
        int count;
    };

    static StatsRecorder& get();

    void addLayoutStat(int scheduleId, const LayoutStat& stat);
    void addMediaStats(int layoutId, int scheduleId, const std::vector<MediaStat>& mediaStats);
    std::string xmlString() const;

private:
    std::vector<Record> records;
};
