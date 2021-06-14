project(commands_tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TESTS_DIRECTORY})

find_package(GTest REQUIRED)
find_library(GMOCK NAMES gmock) # find include dirs

add_executable(${PROJECT_NAME}
    CommandTests.cpp
    main.cpp
)

target_link_libraries(${PROJECT_NAME}
    commands
    ${GMOCK}
    GTest::GTest
)

add_test(NAME CommandsTests COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_TESTS_DIRECTORY})
