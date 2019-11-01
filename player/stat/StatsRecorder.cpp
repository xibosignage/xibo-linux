#include "StatsRecorder.hpp"

#include "common/Parsing.hpp"

void StatsRecorder::addLayoutStat(int scheduleId, int layoutId, const PlayingStat& interval)
{
    long duration = (interval.finished - interval.started).total_seconds();
    Record record{Record::Type::Layout, interval.started, interval.finished, scheduleId, layoutId, {}, duration, 1};
    records_.emplace_back(std::move(record));
}

void StatsRecorder::addMediaStat(int scheduleId, int layoutId, int mediaId, const PlayingStat& interval)
{
    long duration = (interval.finished - interval.started).total_seconds();
    Record record{Record::Type::Media, interval.started, interval.finished, scheduleId, layoutId, mediaId, duration, 1};
    records_.emplace_back(std::move(record));
}

void StatsRecorder::clear()
{
    records_.clear();
}

size_t StatsRecorder::empty() const
{
    return records_.empty();
}

const StatsRecorder::Records& StatsRecorder::records() const
{
    return records_;
}
