#include <boost/optional/optional.hpp>
#include <boost/signals2.hpp>
#include <charconv>
#include <iostream>
#include <sqlite_orm/sqlite_orm.h>

#include "stat/PlayingRecord.hpp"

using namespace sqlite_orm;

namespace sqlite_orm
{
    template <>
    struct type_printer<Stats::PlayingRecord::Type> : public text_printer
    {
    };

    template <>
    struct type_printer<DateTime> : public integer_printer
    {
    };

    template <>
    struct statement_binder<Stats::PlayingRecord::Type>
    {
        std::time_t bind(sqlite3_stmt* stmt, int index, const Stats::PlayingRecord::Type& value)
        {
            return statement_binder<std::string>().bind(stmt, index, Stats::PlayingRecord::typeToString(value));
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
    struct field_printer<Stats::PlayingRecord::Type>
    {
        std::string operator()(const Stats::PlayingRecord::Type& t) const
        {
            return Stats::PlayingRecord::typeToString(t);
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
    struct row_extractor<Stats::PlayingRecord::Type>
    {
        Stats::PlayingRecord::Type extract(const char* row_value)
        {
            if (auto type = Stats::PlayingRecord::typeFromSting(row_value))
            {
                return *type;
            }
            else
            {
                throw std::runtime_error("incorrect record type string (" + std::string(row_value) + ")");
            }
        }

        Stats::PlayingRecord::Type extract(sqlite3_stmt* stmt, int columnIndex)
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
                return DateTime::fromTimestamp(timestamp);
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

#include <chrono>

int main()
{
    auto storage = make_storage("test.sqlite",
                                make_table("stats",
                                           make_column("id", &Stats::PlayingRecord::id, primary_key()),
                                           make_column("type", &Stats::PlayingRecord::type),
                                           make_column("started", &Stats::PlayingRecord::started),
                                           make_column("finished", &Stats::PlayingRecord::finished),
                                           make_column("scheduleId", &Stats::PlayingRecord::scheduleId),
                                           make_column("layoutId", &Stats::PlayingRecord::layoutId),
                                           make_column("mediaId", &Stats::PlayingRecord::mediaId),
                                           make_column("duration", &Stats::PlayingRecord::duration),
                                           make_column("count", &Stats::PlayingRecord::count)));
    auto res = storage.sync_schema();
    for (auto&& [schema, result] : res)
    {
        std::cout << result << std::endl;
    }

    auto start = std::chrono::high_resolution_clock::now();
    {
        //        storage.transaction([&]() {
        //            for (int i = 0; i != 10000; ++i)
        //            {
        //                Stats::PlayingRecord r1{-1,
        //                                        Stats::PlayingRecord::Layout,
        //                                        DateTime::now(),
        //                                        DateTime::now() + DateTime::Hours(1),
        //                                        i + 1,
        //                                        i + 2,
        //                                        i + 3,
        //                                        i + 4,
        //                                        i + 5};
        //                auto id = storage.insert(r1);
        //                //        std::cout << id << std::endl;
        //            }
        //            return true;
        //        });
        auto result = storage.get_all<Stats::PlayingRecord>(limit(500));
        for (auto r : result)
        {
            std::cout << r.id << " " << r.scheduleId << " " << r.layoutId << " " << r.mediaId.value() << " "
                      << r.duration << " " << r.count << std::endl;
        }

        std::cout << "last id " << result.back().Layout
        storage.remove_all<Stats::PlayingRecord>(where(c(&Stats::PlayingRecord::id) <= result.back().id));
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;

    //    auto getr1 = storage.get<Stats::PlayingRecord>(id);
    //    std::cout << getr1.type << " " << getr1.started.string() << " " << getr1.finished.string() << std::endl;
    //    getr1.type = Stats::PlayingRecord::Media;
    //    storage.update(getr1);

    //    {
    //        const char* test = "42 dgsagasdg";
    //        int v = 0;
    //        auto [p, ec] = std::from_chars(test, test + strlen(test), v);
    //        std::cout << v << std::endl;
    //        std::cout << p << " " << (ec == std::errc()) << std::endl;
    //    }
    //    {
    //        const char* test = "fgsdfg";
    //        int v = 0;
    //        auto [p, ec] = std::from_chars(test, test + strlen(test), v);
    //        std::cout << v << std::endl;
    //        std::cout << p << " " << (ec == std::errc()) << std::endl;
    //    }
    //    {
    //        const char* test = "";
    //        int v = 0;
    //        auto [p, ec] = std::from_chars(test, test + strlen(test), v);
    //        std::cout << v << std::endl;
    //        std::cout << p << " " << (ec == std::errc()) << std::endl;
    //    }
    //    {
    //        const char* test = "42";
    //        int v = 0;
    //        auto [p, ec] = std::from_chars(test, test + strlen(test), v);
    //        std::cout << v << std::endl;
    //        std::cout << p << " " << (ec == std::errc()) << std::endl;
    //    }

    //    {
    //        const char* test = nullptr;
    //        int v = 0;
    //        auto [p, ec] = std::from_chars(test, test + strlen(test), v);
    //        std::cout << p << " " << (ec == std::errc()) << std::endl;
    //    }

    return 0;
}
