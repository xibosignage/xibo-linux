#pragma once

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

    void addLayoutStat(int scheduleId, const LayoutStat& stat);
    void addMediaStats(int layoutId, int scheduleId, const std::vector<MediaStat>& mediaStats);
    void clear();
    std::string toXml() const;

private:
    std::vector<Record> records_;
};
