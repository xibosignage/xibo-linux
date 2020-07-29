project(schedule)

add_library(${PROJECT_NAME}
    LayoutQueue.cpp
    LayoutQueue.hpp
    LayoutSchedule.cpp
    LayoutSchedule.hpp
    OverlayLayoutQueue.hpp
    RegularLayoutQueue.cpp
    RegularLayoutQueue.hpp
    ScheduleItem.cpp
    ScheduleItem.hpp
    ScheduleParser.cpp
    ScheduleParser.hpp
    Scheduler.cpp
    Scheduler.hpp
    SchedulerStatus.hpp
    ScheduleSerializer.cpp
    ScheduleSerializer.hpp
)

target_link_libraries(${PROJECT_NAME}
    dt
    fs
    logger
    common
    storage
)

add_subdirectory(tests)
