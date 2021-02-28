#pragma once

#include "DataProvider.hpp"
#include "PlayingRecordType.hpp"

#include "common/dt/DateTime.hpp"

#include <sqlite_orm/sqlite_orm.h>

namespace Stats
{
    class DatabaseProvider : public DataProvider
    {
        struct PrivateData;

    public:
        DatabaseProvider();
        ~DatabaseProvider();

        void save(const PlayingRecordDto& record) override;
        void save(PlayingRecordDtoCollection&& records) override;
        PlayingRecordDtoCollection retrieve(size_t count) const override;
        void removeAll() override;
        void remove(size_t count) override;
        size_t recordsCount() const override;

    private:
        std::unique_ptr<PrivateData> data_;
    };
}
