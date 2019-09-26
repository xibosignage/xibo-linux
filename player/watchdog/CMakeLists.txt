project(watchdog)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

add_executable(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} common)
