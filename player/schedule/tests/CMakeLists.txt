project(schedule_tests)

find_package(GTest REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} schedule ${GTEST_LIBRARIES} ${Boost_LIBRARIES})
