cmake_minimum_required(VERSION 3.14)

project(XiboLinux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

find_package(PkgConfig REQUIRED)
find_package(Boost 1.69 REQUIRED system date_time thread)

# modules
include(FindSonameLib)
include(CopyLibs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -pthread -Wall -Wno-parentheses -Wno-cast-function-type -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs/)

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
configure_file(player/ui.glade ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ui.glade COPYONLY)
configure_file(player/splash.jpg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/splash.jpg COPYONLY)

include_directories(${CMAKE_BINARY_DIR}/generated/)
install(DIRECTORY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_subdirectory(player)
