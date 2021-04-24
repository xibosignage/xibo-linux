#include "stat/storage/DtoConverter.hpp"

using namespace Stats;

RecordDto DtoConverter::dto() const
{
    return dto_;
}

void DtoConverter::visit(const LayoutRecord& record)
{
    fillGenericData(record);

    dto_.layoutId = record.id();
    dto_.type = RecordType::Layout;
}

void DtoConverter::visit(const MediaRecord& record)
{
    fillGenericData(record);

    dto_.layoutId = record.parentId();
    dto_.mediaId = record.id();
    dto_.type = RecordType::Media;
}

void DtoConverter::fillGenericData(const Record& record)
{
    dto_.started = record.started();
    dto_.finished = record.finished();
    dto_.duration = record.duration();
    dto_.count = record.count();
    dto_.scheduleId = record.scheduleId();
}
