#include "stat/storage/DatabaseProvider.hpp"

#include "common/logger/Logging.hpp"
#include "config/AppConfig.hpp"

#include <charconv>

namespace sqlite_orm
{
    template <>
    struct type_printer<Stats::RecordType> : public text_printer
    {
    };

    template <>
    struct type_printer<DateTime> : public integer_printer
    {
    };

    template <>
    struct statement_binder<Stats::RecordType>
    {
        std::time_t bind(sqlite3_stmt* stmt, int index, const Stats::RecordType& value)
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
    struct field_printer<Stats::RecordType>
    {
        std::string operator()(const Stats::RecordType& t) const
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
    struct row_extractor<Stats::RecordType>
    {
        Stats::RecordType extract(const char* row_value)
        {
            if (auto type = Stats::recordTypeFromSting(row_value))
            {
                return *type;
            }
            throw std::runtime_error("incorrect record type string (" + std::string(row_value) + ")");
        }

        Stats::RecordType extract(sqlite3_stmt* stmt, int columnIndex)
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
using namespace std::string_literals;

inline auto initStorage(const FilePath& path)
{
    using namespace sqlite_orm;
    return make_storage(path.string(),
                        make_table("stats",
                                   make_column("id", &RecordDto::id, primary_key()),
                                   make_column("type", &RecordDto::type),
                                   make_column("started", &RecordDto::started),
                                   make_column("finished", &RecordDto::finished),
                                   make_column("scheduleId", &RecordDto::scheduleId),
                                   make_column("layoutId", &RecordDto::layoutId),
                                   make_column("mediaId", &RecordDto::mediaId),
                                   make_column("duration", &RecordDto::duration),
                                   make_column("count", &RecordDto::count)));
}

struct DatabaseProvider::PrivateData
{
    using Storage = decltype(initStorage({}));

    PrivateData(Storage db_) : db(std::move(db_)) {}

    Storage db;
};

DatabaseProvider::DatabaseProvider() : data_(std::make_unique<PrivateData>(initStorage(AppConfig::statsCache())))
{
    try
    {
        data_->db.sync_schema(true);
    }
    catch (const std::exception& e)
    {
        throw Error{"error while syncing schema: "s + e.what()};
    }
}

DatabaseProvider::~DatabaseProvider() {}

void DatabaseProvider::save(const RecordDto& record)
{
    data_->db.insert(record);
}

void DatabaseProvider::save(PlayingRecordDtoCollection&& records)
{
    auto result = data_->db.transaction([&]() {
        try
        {
            for (auto&& record : records)
            {
                data_->db.insert(record);
            }
        }
        catch (std::exception& e)
        {
            return false;
        }
        return true;
    });

    if (!result) throw Error{"database transaction failed"};
}

PlayingRecordDtoCollection DatabaseProvider::retrieve(size_t count) const
{
    return data_->db.get_all<RecordDto>(limit(count));
}

void DatabaseProvider::removeAll()
{
    data_->db.remove_all<RecordDto>();
}

void DatabaseProvider::remove(size_t count)
{
    data_->db.remove_all<RecordDto>(order_by(&RecordDto::id), limit(count));
}

size_t DatabaseProvider::recordsCount() const
{
    return data_->db.count<RecordDto>();
}
