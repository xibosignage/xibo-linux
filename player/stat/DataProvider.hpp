#pragma once

#include "PlayingRecordDto.hpp"
#include "PlayingRecordType.hpp"

namespace Stats
{
    using PlayingRecordDtoCollection = std::vector<PlayingRecordDto>;

    class DataProvider
    {
    public:
        virtual ~DataProvider() = default;
        virtual void save(const PlayingRecordDto& record) = 0;
        virtual void save(PlayingRecordDtoCollection&& records) = 0;
        virtual PlayingRecordDtoCollection retrieve(size_t count) const = 0;
        virtual void removeAll() = 0;
        virtual void remove(size_t count) = 0;
        virtual size_t recordsCount() const = 0;
    };
}
