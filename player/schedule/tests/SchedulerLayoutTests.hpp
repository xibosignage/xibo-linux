#pragma once

#include "schedule/tests/SchedulerReloadTests.hpp"

class SchedulerLayoutTests : public SchedulerTests
{
protected:
    ScheduledLayout notInCacheLayout(int id, int priority)
    {
        auto layout = ScheduleTests::scheduledLayout(id, id, priority);

        ON_CALL(*fileCache_, valid(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(false));
        layout.startDT = DateTime::now() - DateTime::Hours(1);
        layout.endDT = DateTime::now() + DateTime::Hours(1);

        return layout;
    }

    ScheduledLayout notInRangeLayout(int id, int priority)
    {
        auto layout = ScheduleTests::scheduledLayout(id, id, priority);

        ON_CALL(*fileCache_, valid(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(true));
        layout.startDT = DateTime::now() + DateTime::Hours(1);
        layout.endDT = DateTime::now() + DateTime::Hours(2);

        return layout;
    }
};
