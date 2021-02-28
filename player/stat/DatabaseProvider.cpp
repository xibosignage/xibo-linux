#include "DatabaseProvider.hpp"

#include "common/logger/Logging.hpp"

#include <charconv>

namespace sqlite_orm
{
    template <>
    struct type_printer<Stats::PlayingRecordType> : public text_printer
    {
    };

    template <>
    struct type_printer<DateTime> : public integer_printer
    {
    };

    template <>
    struct statement_binder<Stats::PlayingRecordType>
    {
        std::time_t bind(sqlite3_stmt* stmt, int index, const Stats::PlayingRecordType& value)
        {
            return statement_binder<std::string>().bind(stmt, index, Stats::recordTypeToString(value));
        }
    };

    template <>
    struct statement_binder<DateTime>
    {
        std::time_t bind(sqlite3_stmt* stmt, int index, const DateTime& value)
        {
            return statement_binder<std::time_t>().bind(stmt, index, value.timestamp());
        }
    };

    template <>
    struct field_printer<Stats::PlayingRecordType>
    {
        std::string operator()(const Stats::PlayingRecordType& t) const
        {
            return Stats::recordTypeToString(t);
        }
    };

    template <>
    struct field_printer<DateTime>
    {
        std::time_t operator()(const DateTime& dt) const
        {
            return dt.timestamp();
        }
    };

    template <>
    struct row_extractor<Stats::PlayingRecordType>
    {
        Stats::PlayingRecordType extract(const char* row_value)
        {
            if (auto type = Stats::recordTypeFromSting(row_value))
            {
                return *type;
            }
            throw std::runtime_error("incorrect record type string (" + std::string(row_value) + ")");
        }

        Stats::PlayingRecordType extract(sqlite3_stmt* stmt, int columnIndex)
        {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char*)str);
        }
    };

    template <>
    struct row_extractor<DateTime>
    {
        DateTime extract(const char* row_value)
        {
            std::time_t timestamp;
            auto [p, ec] = std::from_chars(row_value, row_value + strlen(row_value), timestamp);
            if (ec == std::errc())
            {
                return DateTime::utcFromTimestamp(timestamp);
            }
            throw std::runtime_error("incorrect timestamp string (" + std::string(row_value) + ")");
        }

        DateTime extract(sqlite3_stmt* stmt, int columnIndex)
        {
            auto str = sqlite3_column_text(stmt, columnIndex);
            return this->extract((const char*)str);
        }
    };
}

using namespace Stats;
using namespace sqlite_orm;

inline auto initStorage(const std::string& path)
{
    using namespace sqlite_orm;
    return make_storage(path,
                        make_table("stats",
                                   make_column("id", &PlayingRecordDto::id, primary_key()),
                                   make_column("type", &PlayingRecordDto::type),
                                   make_column("started", &PlayingRecordDto::started),
                                   make_column("finished", &PlayingRecordDto::finished),
                                   make_column("scheduleId", &PlayingRecordDto::scheduleId),
                                   make_column("layoutId", &PlayingRecordDto::layoutId),
                                   make_column("mediaId", &PlayingRecordDto::mediaId),
                                   make_column("duration", &PlayingRecordDto::duration),
                                   make_column("count", &PlayingRecordDto::count)));
}

struct DatabaseProvider::PrivateData
{
    using Storage = decltype(initStorage({}));

    PrivateData(Storage db_) : db(std::move(db_)) {}

    Storage db;
};

// TODO: probably move to config
DatabaseProvider::DatabaseProvider() : data_(std::make_unique<PrivateData>(initStorage("stats.sqlite")))
{
    data_->db.sync_schema(true);
}

DatabaseProvider::~DatabaseProvider() { }

void DatabaseProvider::save(const PlayingRecordDto& record)
{
    data_->db.insert(record);
}

void DatabaseProvider::save(PlayingRecordDtoCollection&& records)
{
    data_->db.transaction([&]() {
        try
        {
            for (auto&& record : records)
            {
                data_->db.insert(record);
            }
        }
        catch (std::exception& e)
        {
            Log::error("[Stats::Database] Error while saving records to database: {}", e.what());
            return false;
        }
        return true;
    });
}

PlayingRecordDtoCollection DatabaseProvider::retrieve(size_t count) const
{
    return data_->db.get_all<PlayingRecordDto>(limit(count));
}

void DatabaseProvider::removeAll()
{
    data_->db.remove_all<PlayingRecordDto>();
}

void DatabaseProvider::remove(size_t count)
{
    data_->db.remove_all<PlayingRecordDto>(order_by(&PlayingRecordDto::id), limit(count));
}

size_t DatabaseProvider::recordsCount() const
{
    return data_->db.count<PlayingRecordDto>();
}
