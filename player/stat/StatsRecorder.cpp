#include "StatsRecorder.hpp"
#include "StatsFormatter.hpp"

#include "common/Parsing.hpp"

void StatsRecorder::addLayoutStat(int scheduleId, int layoutId, const PlayingStat& interval)
{
    long duration = (interval.finished - interval.started).total_seconds();
    Record record{Record::Type::Layout, interval.started, interval.finished, scheduleId, layoutId, {}, duration, 1};
    records_.add(std::move(record));
}

void StatsRecorder::addMediaStat(int scheduleId, int layoutId, int mediaId, const PlayingStat& interval)
{
    long duration = (interval.finished - interval.started).total_seconds();
    Record record{Record::Type::Media, interval.started, interval.finished, scheduleId, layoutId, mediaId, duration, 1};
    records_.add(std::move(record));
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

std::string StatsRecorder::Records::string() const
{
    StatsFormatter formatter;
    return formatter.toXml(*this);
}

StatsRecorder::Records::iterator StatsRecorder::Records::begin()
{
    return records_.begin();
}

StatsRecorder::Records::iterator StatsRecorder::Records::end()
{
    return records_.end();
}

StatsRecorder::Records::const_iterator StatsRecorder::Records::begin() const
{
    return records_.begin();
}

StatsRecorder::Records::const_iterator StatsRecorder::Records::end() const
{
    return records_.end();
}

void StatsRecorder::Records::add(StatsRecorder::Record&& record)
{
    records_.emplace_back(std::move(record));
}

void StatsRecorder::Records::clear()
{
    records_.clear();
}

bool StatsRecorder::Records::empty() const
{
    return records_.empty();
}
