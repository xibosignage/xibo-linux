#include "MediaRecord.hpp"

#include "RecordVisitor.hpp"

using namespace Stats;

std::unique_ptr<MediaRecord> MediaRecord::create(int scheduleId,
                                                 int parentId,
                                                 int id,
                                                 const PlayingTime& interval,
                                                 int count)
{
    return std::unique_ptr<MediaRecord>(new MediaRecord{scheduleId, parentId, id, interval, count});
}

void MediaRecord::apply(RecordVisitor& visitor)
{
    visitor.visit(*this);
}