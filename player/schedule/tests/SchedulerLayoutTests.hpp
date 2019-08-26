#pragma once

#include "SchedulerReloadTests.hpp"

class SchedulerLayoutTests : public SchedulerTests
{
protected:
    ScheduledLayout notInCacheLayout(int id, int priority)
    {
        auto layout = ScheduleTests::scheduledLayout(id, id, priority);

        ON_CALL(*fileCache, valid(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(false));
        layout.startDT = DateTimeProvider::now() - DateTimeHours(1);
        layout.endDT = DateTimeProvider::now() + DateTimeHours(1);

        return layout;}

    ScheduledLayout notInRangeLayout(int id, int priority)
    {
        auto layout = ScheduleTests::scheduledLayout(id, id, priority);

        ON_CALL(*fileCache, valid(std::to_string(id) + ".xlf")).WillByDefault(testing::Return(true));
        layout.startDT = DateTimeProvider::now() + DateTimeHours(1);
        layout.endDT = DateTimeProvider::now() + DateTimeHours(2);

        return layout;
    }

};
