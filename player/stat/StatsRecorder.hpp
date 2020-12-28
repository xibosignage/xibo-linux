#pragma once

#include "stat/PlayingStat.hpp"

#include <boost/noncopyable.hpp>
#include <boost/optional/optional.hpp>

class StatsRecorder : private boost::noncopyable
{
public:
    struct Record
    {
        enum Type
        {
            Layout,
            Media
        };

        Type type;
        DateTime started;
        DateTime finished;
        int scheduleId;
        int layoutId;
        boost::optional<int> mediaId;
        long duration;
        int count;
    };

    class Records
    {
    public:
        using iterator = std::vector<Record>::iterator;
        using const_iterator = std::vector<Record>::const_iterator;

        void add(Record&& record);
        void clear();
        bool empty() const;
        std::string string() const;

        iterator begin();
        iterator end();
        const_iterator begin() const;
        const_iterator end() const;

    private:
        std::vector<Record> records_;
    };

    void addLayoutStat(int layoutId, int scheduleId, const PlayingStat& interval);
    void addMediaStat(int scheduleId, int layoutId, int mediaId, const PlayingStat& interval);
    void clear();
    size_t empty() const;
    const Records& records() const;

private:
    Records records_;
};
