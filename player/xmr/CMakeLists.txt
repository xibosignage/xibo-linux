project(xmr)

pkg_check_modules(ZeroMQ REQUIRED libzmq>=4.3.2)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})
file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

add_library(${PROJECT_NAME} ${SOURCES})
target_link_libraries(${PROJECT_NAME} common ${ZeroMQ_LINK_LIBRARIES})

copy_libs("${ZeroMQ_LINK_LIBRARIES}")
