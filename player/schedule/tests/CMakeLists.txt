project(schedule_tests)

find_package(GTest REQUIRED)
find_library(GMOCK NAMES gmock)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp ${CMAKE_CURRENT_SOURCE_DIR}/mocks/*.*pp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} schedule ${GMOCK} ${GTEST_LIBRARIES} ${Boost_LIBRARIES})

add_test(NAME SchedulerTests COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_TESTS_DIRECTORY})
