project(common_tests)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_TESTS_DIRECTORY})

find_package(GTest REQUIRED)
find_library(GMOCK NAMES gmock)

add_executable(${PROJECT_NAME}
    ColorConverterTests.cpp
    ColorConverterTests.hpp
    main.cpp
    UriTests.cpp
    UriTests.hpp
)
target_link_libraries(${PROJECT_NAME}
    types
    ${GMOCK}
    GTest::GTest
)

add_test(NAME CommonTests COMMAND ${PROJECT_NAME} WORKING_DIRECTORY ${CMAKE_TESTS_DIRECTORY})
