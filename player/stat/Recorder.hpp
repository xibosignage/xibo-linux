#pragma once

#include "storage/DataProvider.hpp"
#include "records/Records.hpp"
#include "records/LayoutRecord.hpp"
#include "PlayingTime.hpp"

#include "common/PlayerRuntimeError.hpp"

#include <boost/noncopyable.hpp>
#include <mutex>

namespace Stats
{
    class Recorder : private boost::noncopyable
    {
        DECLARE_EXCEPTION(Stats::Recorder)
    public:
        Recorder();

        void addLayoutRecord(std::unique_ptr<LayoutRecord> record);
        void addMediaRecords(MediaRecords&& records);
        void removeFromQueue(size_t count);
        size_t recordsCount() const;
        Records records(size_t count) const;

    private:
        std::unique_ptr<Record> createPlayingRecord(const RecordDto& data) const;
        void checkIfCacheIsValid() const;

    private:
        mutable std::mutex locker_;
        std::unique_ptr<DataProvider> dataProvider_;
    };

}
