#pragma once

#include "DataProvider.hpp"
#include "stat/records/RecordType.hpp"

#include "common/PlayerRuntimeError.hpp"
#include "common/dt/DateTime.hpp"

#include <sqlite_orm/sqlite_orm.h>

namespace Stats
{
    class DatabaseProvider : public DataProvider
    {
        struct PrivateData;

        DECLARE_EXCEPTION(Stats::Database)
    public:
        DatabaseProvider();
        ~DatabaseProvider();

        void save(const RecordDto& record) override;
        void save(PlayingRecordDtoCollection&& records) override;
        PlayingRecordDtoCollection retrieve(size_t count) const override;
        void removeAll() override;
        void remove(size_t count) override;
        size_t recordsCount() const override;

    private:
        std::unique_ptr<PrivateData> data_;
    };
}
