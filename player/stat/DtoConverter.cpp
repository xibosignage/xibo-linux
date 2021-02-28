#include "DtoConverter.hpp"

using namespace Stats;

PlayingRecordDto DtoConverter::dto() const
{
    return dto_;
}

void DtoConverter::visit(const LayoutPlayingRecord& record)
{
    fillGenericData(record);

    dto_.layoutId = record.id();
    dto_.type = PlayingRecordType::Layout;
}

void DtoConverter::visit(const MediaPlayingRecord& record)
{
    fillGenericData(record);

    dto_.layoutId = record.parentId();
    dto_.mediaId = record.id();
    dto_.type = PlayingRecordType::Media;
}

void DtoConverter::fillGenericData(const PlayingRecord& record)
{
    dto_.started = record.started();
    dto_.finished = record.finished();
    dto_.duration = record.duration();
    dto_.count = record.count();
    dto_.scheduleId = record.scheduleId();
}
