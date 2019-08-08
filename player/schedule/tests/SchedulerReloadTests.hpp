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

    struct RegularTag { };
    struct OverlayTag { };

    template<typename Tag, typename... Ids>
    LayoutSchedule makeSchedule(int defaultId, Ids... regularIds)
    {
        LayoutSchedule schedule;

        schedule.defaultLayout = defaultLayout(defaultId);

        if constexpr(std::is_same_v<Tag, RegularTag>)
        {
            (schedule.regularLayouts.emplace_back(validLayout(regularIds, DefaultPriority)), ...);
        }
        else
        {
            (schedule.overlayLayouts.emplace_back(validLayout(regularIds, DefaultPriority)), ...);
        }

        return schedule;
    }

    template<typename... Ids>
    LayoutSchedule makeRegularSchedule(int defaultId, Ids... regularIds)
    {
        return makeSchedule<RegularTag>(defaultId, regularIds...);
    }

    template<typename... Ids>
    LayoutSchedule makeOverlaySchedule(Ids... regularIds)
    {
        return makeSchedule<OverlayTag>(DefaultId, regularIds...);
    }

protected:
    std::unique_ptr<FakeFileCache> fileCache;

};
