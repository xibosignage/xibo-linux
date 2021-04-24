#include "Record.hpp"

using namespace Stats;

Record::Record(int scheduleId, int id, const PlayingTime& interval, int count) :
    Record(scheduleId, InvalidId, id, interval, count)
{
}

Record::Record(int scheduleId, int parentId, int id, const PlayingTime& interval, int count) :
    scheduleId_{scheduleId}, parentId_{parentId}, id_{id}, interval_{interval}, count_{count}
{
}

DateTime Record::started() const
{
    return interval_.started;
}

DateTime Record::finished() const
{
    return interval_.finished;
}

int Record::scheduleId() const
{
    return scheduleId_;
}

int Record::parentId() const
{
    return parentId_;
}

int Record::id() const
{
    return id_;
}

long Record::duration() const
{
    return (interval_.finished - interval_.started).total_seconds();
}

int Record::count() const
{
    return count_;
}
