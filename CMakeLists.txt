cmake_minimum_required(VERSION 3.14.1)

project(XiboLinux)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

find_package(PkgConfig REQUIRED)
find_package(Boost 1.66 REQUIRED system program_options date_time thread)

# modules
include(FindSonameLib)
include(CopyLibs)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -O0 -pthread -Wall -Wno-parentheses -Wno-cast-function-type -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/libs/)

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
configure_file(player/ui.glade ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ui.glade COPYONLY)

include_directories(${CMAKE_BINARY_DIR}/generated/)
install(DIRECTORY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})

add_subdirectory(player)
