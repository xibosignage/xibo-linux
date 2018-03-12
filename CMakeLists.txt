cmake_minimum_required(VERSION 3.5.1)

execute_process(COMMAND git rev-list --all --count COMMAND xargs echo -n WORKING_DIRECTORY ${CMAKE_SOURCE_DIR} OUTPUT_VARIABLE GIT_REVISION)
project(XiboLinux VERSION 0.2.0.${GIT_REVISION})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config was not found")
endif()

pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0)
function(find_libs_path libraries_list output_var)
    foreach(lib ${libraries_list})
        find_library(${lib}_path NAMES ${lib})
        set(libs_path ${libs_path} ${${lib}_path})
    endforeach(lib)
    set(${output_var} ${libs_path} PARENT_SCOPE)
endfunction()
find_libs_path("${Gtkmm_LIBRARIES}" Gtkmm_FULL_LIBRARIES)

find_package(Boost 1.58 REQUIRED filesystem program_options)

include(CTest)

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${CMAKE_BINARY_DIR}/generated/)

add_subdirectory(Player)
add_subdirectory(Watchdog)
