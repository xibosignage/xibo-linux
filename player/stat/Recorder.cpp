#include "Recorder.hpp"

#include "DatabaseProvider.hpp"
#include "DtoConverter.hpp"
#include "XmlFormatter.hpp"

using namespace Stats;

Recorder::Recorder() : dataProvider_(std::make_unique<DatabaseProvider>()) {}

void Recorder::addLayoutRecord(std::unique_ptr<LayoutPlayingRecord> record)
{
    std::lock_guard<std::mutex> lock(locker_);

    DtoConverter converter;
    record->apply(converter);

    dataProvider_->save(converter.dto());
}

void Recorder::addMediaRecords(MediaPlayingRecords&& records)
{
    std::lock_guard<std::mutex> lock(locker_);

    PlayingRecordDtoCollection dataCollection;

    for (auto&& record : records)
    {
        DtoConverter converter;
        record->apply(converter);

        dataCollection.emplace_back(converter.dto());
    }

    dataProvider_->save(std::move(dataCollection));
}

void Recorder::removeFromQueue(size_t count)
{
    std::lock_guard<std::mutex> lock(locker_);

    dataProvider_->remove(count);
}

size_t Recorder::recordsCount() const
{
    std::lock_guard<std::mutex> lock(locker_);

    return dataProvider_->recordsCount();
}

PlayingRecords Recorder::records(size_t count) const
{
    std::lock_guard<std::mutex> lock(locker_);

    PlayingRecords records;

    for (auto&& data : dataProvider_->retrieve(count))
    {
        auto record = createPlayingRecord(data);
        if (record)
        {
            records.add(std::move(record));
        }
    }

    return records;
}

std::unique_ptr<PlayingRecord> Recorder::createPlayingRecord(const PlayingRecordDto& data) const
{
    PlayingTime interval{data.started, data.finished};

    switch (data.type)
    {
        case PlayingRecordType::Layout:
            return LayoutPlayingRecord::create(data.scheduleId, data.layoutId, interval, data.count);
        case PlayingRecordType::Media:
            return MediaPlayingRecord::create(
                data.scheduleId, data.layoutId, data.mediaId.value(), interval, data.count);

        default: break;
    }

    return nullptr;
}