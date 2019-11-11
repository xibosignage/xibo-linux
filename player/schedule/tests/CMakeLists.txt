project(schedule_tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TESTS_DIRECTORY})

find_package(GTest REQUIRED)
find_library(GMOCK NAMES gmock) # find include dirs

add_executable(${PROJECT_NAME}
    Common.cpp
    Common.hpp
    FakeFileCache.hpp
    LayoutQueueTests.cpp
    LayoutQueueTests.hpp
    LayoutScheduleTests.cpp
    main.cpp
    ScheduleParserTests.cpp
    SchedulerLayoutTests.hpp
    SchedulerOverlayTests.cpp
    SchedulerRegularTests.cpp
    SchedulerReloadTests.cpp
    SchedulerReloadTests.hpp
    ScheduleSerializerTests.cpp
)
target_link_libraries(${PROJECT_NAME}
    schedule
    ${GMOCK}
    ${GTEST_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTEST_INCLUDE_DIRS}
)

add_test(NAME SchedulerTests COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_TESTS_DIRECTORY})
