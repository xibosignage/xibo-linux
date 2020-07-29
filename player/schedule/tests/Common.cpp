#include "Common.hpp"

namespace ScheduleTests
{
    const DateTime StartDt{DateTime::Date(1970, 1, 1), DateTime::Time(1, 0, 0)};
    const DateTime EndDt{DateTime::Date(2038, 1, 19), DateTime::Time(3, 14, 7)};

    ScheduledLayout scheduledLayout(int scheduledId, int id, int priority)
    {
        return ScheduledLayout{scheduledId, id, priority, {}, {}, {}};
    }

    ScheduledLayout scheduledLayout()
    {
        return ScheduledLayout{DefaultScheduleId, DefaultTestId, DefaultTestPriority, {}, {}, {}};
    }

    DefaultScheduledLayout defaultLayout()
    {
        DefaultScheduledLayout layout;

        layout.id = 1;
        layout.dependants.emplace_back("defaultLocal1.txt");
        layout.dependants.emplace_back("defaultLocal2.txt");

        return layout;
    }

    LayoutList regularLayouts()
    {
        LayoutList list;

        ScheduledLayout layout;
        layout.scheduleId = 2;
        layout.id = 3;
        layout.priority = 1;
        layout.startDT = StartDt;
        layout.endDT = EndDt;
        layout.dependants.emplace_back("local1.txt");
        layout.dependants.emplace_back("local2.txt");

        list.emplace_back(layout);

        return list;
    }

    LayoutList overlayLayouts()
    {
        LayoutList list;

        ScheduledLayout layout;
        layout.scheduleId = 1;
        layout.id = 2;
        layout.priority = 0;
        layout.startDT = StartDt;
        layout.endDT = EndDt;

        list.emplace_back(layout);

        return list;
    }

    LayoutDependants globalDependants()
    {
        LayoutDependants dependants;

        dependants.emplace_back("global1.txt");
        dependants.emplace_back("global2.txt");

        return dependants;
    }

    LayoutSchedule schedule()
    {
        LayoutSchedule schedule;

        schedule.generatedTime = DateTime(DateTime::Date(2019, 1, 8), DateTime::Time(8, 0, 0));
        schedule.defaultLayout = defaultLayout();
        schedule.regularLayouts = regularLayouts();
        schedule.overlayLayouts = overlayLayouts();
        schedule.globalDependants = globalDependants();

        return schedule;
    }
}
