#include "LayoutRecord.hpp"

#include "RecordVisitor.hpp"

using namespace Stats;

std::unique_ptr<LayoutRecord> LayoutRecord::create(int scheduleId, int id, const PlayingTime& interval, int count)
{
    return std::unique_ptr<LayoutRecord>(new LayoutRecord{scheduleId, id, interval, count});
}

void LayoutRecord::apply(RecordVisitor& visitor)
{
    visitor.visit(*this);
}