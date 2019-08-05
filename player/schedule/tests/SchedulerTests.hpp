#pragma once

#include <gtest/gtest.h>

#include "FakeFileCache.hpp"
#include "Common.hpp"
#include "common/dt/DateTimeProvider.hpp"

class Scheduler;

class SchedulerTests : public testing::Test
{
public:
    std::unique_ptr<Scheduler> construct()
    {
        return std::make_unique<Scheduler>(*fileCache);
    }

    void SetUp() override
    {
        fileCache = std::make_unique<testing::NiceMock<FakeFileCache>>();
    }

    void TearDown() override
    {
        fileCache.reset();
    }

protected:
    DefaultScheduledLayout defaultLayout(int id)
    {
        DefaultScheduledLayout layout;

        ON_CALL(*this->fileCache, cached(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(true));
        layout.id = id;

        return layout;
    }

    ScheduledLayout validLayout(int id, int priority)
    {
        auto layout = ScheduleTests::scheduledLayout(id, id, priority);

        ON_CALL(*fileCache, cached(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(true));
        layout.startDT = DateTimeProvider::now() - DateTimeHours(1);
        layout.endDT = DateTimeProvider::now() + DateTimeHours(1);

        return layout;
    }


    template<typename... Ids>
    LayoutSchedule makeSchedule(int defaultId, Ids... regularIds)
    {
        LayoutSchedule schedule;

        schedule.defaultLayout = defaultLayout(defaultId);
        (schedule.regularLayouts.emplace_back(validLayout(regularIds, DefaultPriority)), ...);

        return schedule;
    }


protected:
    std::unique_ptr<FakeFileCache> fileCache;

};
