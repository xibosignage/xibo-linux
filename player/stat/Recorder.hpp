#pragma once

#include "DataProvider.hpp"
#include "PlayingRecords.hpp"
#include "PlayingTime.hpp"

#include <boost/noncopyable.hpp>
#include <mutex>

namespace Stats
{
    class Recorder : private boost::noncopyable
    {
    public:
        Recorder();
        
        void addLayoutRecord(std::unique_ptr<LayoutPlayingRecord> record);
        void addMediaRecords(MediaPlayingRecords&& records);
        void removeFromQueue(size_t count);
        size_t recordsCount() const;
        PlayingRecords records(size_t count) const;

    private:
        std::unique_ptr<PlayingRecord> createPlayingRecord(const PlayingRecordDto& data) const;

    private:
        mutable std::mutex locker_;
        std::unique_ptr<DataProvider> dataProvider_;
    };

}
