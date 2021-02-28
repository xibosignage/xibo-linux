#include "PlayingRecord.hpp"

#include "IRecordVisitor.hpp"

using namespace Stats;

PlayingRecord::PlayingRecord(int scheduleId, int id, const PlayingTime& interval, int count) :
    PlayingRecord(scheduleId, InvalidId, id, interval, count)
{
}

PlayingRecord::PlayingRecord(int scheduleId, int parentId, int id, const PlayingTime& interval, int count) :
    scheduleId_{scheduleId}, parentId_{parentId}, id_{id}, interval_{interval}, count_{count}
{
}

DateTime PlayingRecord::started() const
{
    return interval_.started;
}

DateTime PlayingRecord::finished() const
{
    return interval_.finished;
}

int PlayingRecord::scheduleId() const
{
    return scheduleId_;
}

int PlayingRecord::parentId() const
{
    return parentId_;
}

int PlayingRecord::id() const
{
    return id_;
}

long PlayingRecord::duration() const
{
    return (interval_.finished - interval_.started).total_seconds();
}

int PlayingRecord::count() const
{
    return count_;
}

std::unique_ptr<LayoutPlayingRecord> LayoutPlayingRecord::create(int scheduleId,
                                                                 int id,
                                                                 const PlayingTime& interval,
                                                                 int count)
{
    return std::unique_ptr<LayoutPlayingRecord>(new LayoutPlayingRecord{scheduleId, id, interval, count});
}

void LayoutPlayingRecord::apply(RecordVisitor& visitor)
{
    visitor.visit(*this);
}

std::unique_ptr<MediaPlayingRecord> MediaPlayingRecord::create(int scheduleId,
                                                               int parentId,
                                                               int id,
                                                               const PlayingTime& interval,
                                                               int count)
{
    return std::unique_ptr<MediaPlayingRecord>(new MediaPlayingRecord{scheduleId, parentId, id, interval, count});
}

void MediaPlayingRecord::apply(RecordVisitor& visitor)
{
    visitor.visit(*this);
}
