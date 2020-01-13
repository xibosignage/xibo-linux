project(common_tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TESTS_DIRECTORY})

find_package(GTest REQUIRED)
find_library(GMOCK NAMES gmock)

file(GLOB SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp
)
add_executable(${PROJECT_NAME}
    ${SOURCES}
)
target_link_libraries(${PROJECT_NAME}
    common ${GMOCK}
    ${GTEST_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_test(NAME CommonTests COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_TESTS_DIRECTORY})
