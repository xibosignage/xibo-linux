cmake_minimum_required(VERSION 3.8)

execute_process(COMMAND git rev-list v0.2-a..HEAD --count COMMAND xargs echo -n WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE GIT_REVISION)
project(XiboLinux VERSION 0.2.1.${GIT_REVISION})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/modules")

find_package(PkgConfig REQUIRED)
find_package(Boost 1.58 REQUIRED system filesystem program_options)

# modules
include(FindSonameLibs)
include(FindLinkLibs)
include(CopyLibs)
include(CTest)

enable_testing()
pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0>=3.18.0)
find_link_libs("${Gtkmm_LIBRARIES}" Gtkmm_LINK_LIBRARIES)

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${Gtkmm_INCLUDE_DIRS} ${CMAKE_BINARY_DIR}/generated/)

add_subdirectory(player)
