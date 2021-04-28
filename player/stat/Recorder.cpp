#include "Recorder.hpp"

#include "stat/records/XmlFormatter.hpp"
#include "stat/storage/DatabaseProvider.hpp"
#include "stat/storage/DtoConverter.hpp"

#include "common/logger/Logging.hpp"

using namespace Stats;
using namespace std::string_literals;

Recorder::Recorder()
{
    try
    {
        dataProvider_ = std::make_unique<DatabaseProvider>();
    }
    catch (const std::exception& e)
    {
        Log::error("[Stats::Recorder] Database: {}. Stats won't be cached", e.what());
    }
}

void Recorder::addLayoutRecord(std::unique_ptr<LayoutRecord> record)
{
    try
    {
        std::lock_guard<std::mutex> lock(locker_);

        checkIfCacheIsValid();

        DtoConverter converter;
        record->apply(converter);

        dataProvider_->save(converter.dto());
    }
    catch (const std::exception& e)
    {
        throw Error{"Failed to add layout record to cache: "s + e.what()};
    }
}

void Recorder::addMediaRecords(MediaRecords&& records)
{
    try
    {
        std::lock_guard<std::mutex> lock(locker_);

        checkIfCacheIsValid();

        PlayingRecordDtoCollection dataCollection;
        for (auto&& record : records)
        {
            DtoConverter converter;
            record->apply(converter);

            dataCollection.emplace_back(converter.dto());
        }
        dataProvider_->save(std::move(dataCollection));
    }
    catch (const std::exception& e)
    {
        throw Error{"Failed to add media records to cache: "s + e.what()};
    }
}

void Recorder::removeFromQueue(size_t count)
{
    try
    {
        std::lock_guard<std::mutex> lock(locker_);

        checkIfCacheIsValid();

        dataProvider_->remove(count);
    }
    catch (const std::exception& e)
    {
        throw Error{"Failed to remove records from cache: "s + e.what()};
    }
}

size_t Recorder::recordsCount() const
{
    try
    {
        std::lock_guard<std::mutex> lock(locker_);

        checkIfCacheIsValid();

        return dataProvider_->recordsCount();
    }
    catch (const std::exception& e)
    {
        throw Error{"Failed to count records from cache: "s + e.what()};
    }
}

Records Recorder::records(size_t count) const
{
    try
    {
        std::lock_guard<std::mutex> lock(locker_);

        checkIfCacheIsValid();

        Records records;
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
    catch (const std::exception& e)
    {
        throw Error{"Failed to retrieve records from cache: "s + e.what()};
    }
}

std::unique_ptr<Record> Recorder::createPlayingRecord(const RecordDto& data) const
{
    PlayingTime interval{data.started, data.finished};

    switch (data.type)
    {
        case RecordType::Layout: return LayoutRecord::create(data.scheduleId, data.layoutId, interval, data.count);
        case RecordType::Media:
            return MediaRecord::create(data.scheduleId, data.layoutId, data.mediaId.value(), interval, data.count);

        default: break;
    }

    return nullptr;
}

void Recorder::checkIfCacheIsValid() const
{
    if (!dataProvider_) throw std::runtime_error{"cache is not initialized"};
}