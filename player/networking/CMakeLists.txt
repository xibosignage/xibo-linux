project(networking)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_library(ZeroMQ zmq)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(${PROJECT_NAME} stdc++fs ssl crypto ${ZeroMQ} ${Boost_LIBRARIES})
