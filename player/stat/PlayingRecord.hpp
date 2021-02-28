#pragma once

#include "PlayingRecordType.hpp"
#include "PlayingTime.hpp"

#include <optional>

namespace Stats
{
    struct RecordVisitor;

    const int InvalidId = -1;

    class PlayingRecord
    {
    public:
        virtual ~PlayingRecord() = default;
        virtual void apply(RecordVisitor& visitor) = 0;

        int scheduleId() const;
        int parentId() const;
        int id() const;
        DateTime started() const;
        DateTime finished() const;
        int count() const;

        long duration() const;

    protected:
        PlayingRecord(int scheduleId, int id, const PlayingTime& interval, int count);
        PlayingRecord(int scheduleId, int parentId, int id, const PlayingTime& interval, int count);

    private:
        int scheduleId_;
        int parentId_;
        int id_;
        PlayingTime interval_;
        int count_;
    };

    class LayoutPlayingRecord : public PlayingRecord
    {
        using PlayingRecord::PlayingRecord;

    public:
        static std::unique_ptr<LayoutPlayingRecord> create(int scheduleId,
                                                           int id,
                                                           const PlayingTime& interval,
                                                           int count = 1);
        void apply(RecordVisitor& visitor) override;
    };

    class MediaPlayingRecord : public PlayingRecord
    {
        using PlayingRecord::PlayingRecord;

    public:
        static std::unique_ptr<MediaPlayingRecord> create(int scheduleId,
                                                          int parentId,
                                                          int id,
                                                          const PlayingTime& interval,
                                                          int count = 1);

        void apply(RecordVisitor& visitor) override;
    };
}
