cmake_minimum_required(VERSION 3.10)

execute_process(COMMAND git rev-list --all --count OUTPUT_VARIABLE GIT_REVISION)
string(REGEX REPLACE "\n$" "" GIT_REVISION "${GIT_REVISION}")

project(XiboLinux VERSION 0.2.0.${GIT_REVISION})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config was not found")
endif()

find_package(Boost 1.58 REQUIRED filesystem program_options)

include(CTest)

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${CMAKE_BINARY_DIR}/generated/)

add_subdirectory(Player)
add_subdirectory(Watchdog)
