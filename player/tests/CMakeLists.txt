CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

PROJECT(unit_tests)

find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-zero-as-null-pointer-constant")

find_library(GMOCK NAMES gmock)

add_executable(${PROJECT_NAME} main.cpp MainLayoutTests.cpp)
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} player_lib ${GMOCK} ${GTEST_LIBRARIES} gst_wrapper xibovideosink ${Gtkmm_LINK_LIBRARIES} ${GStreamer_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES} ${Boost_LIBRARIES})
