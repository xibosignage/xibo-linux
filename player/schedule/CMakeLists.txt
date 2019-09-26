project(schedule)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

add_subdirectory(tests)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} common)


