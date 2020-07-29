#pragma once

#include "common/dt/DateTime.hpp"
#include "schedule/tests/Common.hpp"
#include "schedule/tests/FakeFileCache.hpp"

#include <gtest/gtest.h>

class Scheduler;

class SchedulerTests : public testing::Test
{
public:
    std::unique_ptr<Scheduler> construct()
    {
        return std::make_unique<Scheduler>(*fileCache_);
    }

    void SetUp() override
    {
        fileCache_ = std::make_unique<testing::NiceMock<FakeFileCache>>();
    }

    void TearDown() override
    {
        fileCache_.reset();
    }

protected:
    DefaultScheduledLayout defaultLayout(int id)
    {
        DefaultScheduledLayout layout;

        ON_CALL(*this->fileCache_, valid(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(true));
        layout.id = id;

        return layout;
    }

    ScheduledLayout validLayout(int id, int priority)
    {
        auto layout = ScheduleTests::scheduledLayout(id, id, priority);

        ON_CALL(*fileCache_, valid(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(true));
        layout.startDT = DateTime::now() - DateTime::Hours(1);
        layout.endDT = DateTime::now() + DateTime::Hours(1);

        return layout;
    }

    struct RegularTag
    {
    };
    struct OverlayTag
    {
    };

    template <typename Tag, typename... Ids>
    LayoutSchedule makeSchedule(int defaultId, Ids... regularIds)
    {
        LayoutSchedule schedule;

        schedule.defaultLayout = defaultLayout(defaultId);

        if constexpr (std::is_same_v<Tag, RegularTag>)
        {
            (schedule.regularLayouts.emplace_back(validLayout(regularIds, DefaultTestPriority)), ...);
        }
        else
        {
            (schedule.overlayLayouts.emplace_back(validLayout(regularIds, DefaultTestPriority)), ...);
        }

        return schedule;
    }

    template <typename... Ids>
    LayoutSchedule makeRegularSchedule(int defaultId, Ids... regularIds)
    {
        return makeSchedule<RegularTag>(defaultId, regularIds...);
    }

    template <typename... Ids>
    LayoutSchedule makeOverlaySchedule(Ids... regularIds)
    {
        return makeSchedule<OverlayTag>(DefaultTestId, regularIds...);
    }

protected:
    std::unique_ptr<FakeFileCache> fileCache_;
};
