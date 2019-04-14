project(gstwrapper)

file(GLOB_RECURSE SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)

add_library(${PROJECT_NAME} SHARED ${SOURCES})
