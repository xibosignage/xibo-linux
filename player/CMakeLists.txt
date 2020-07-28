cmake_minimum_required(VERSION 3.14.1)

project(player)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0>=3.22.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4>=2.56.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_TESTS_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests/)

add_compile_options(
    -static-libstdc++ -pthread -Wall -Wno-parentheses -Wno-cast-function-type -W -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
    "$<$<CONFIG:RELEASE>:-O2>"
    "$<$<CONFIG:DEBUG>:-g3;-O0>"
)

add_compile_definitions(
    BOOST_THREAD_PROVIDES_FUTURE
    BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
    BOOST_STACKTRACE_USE_ADDR2LINE
    BOOST_STACKTRACE_USE_BACKTRACE
)

option(SNAP_BUILD "Use SNAP environment during build" OFF)
if(SNAP_BUILD)
    add_compile_definitions(SNAP_ENABLED)
endif(SNAP_BUILD)

option(GTK_UI "Use GTK library for UI" ON)
if(GTK_UI)
    add_compile_definitions(USE_GTK)
endif(GTK_UI)

add_executable(${PROJECT_NAME}
    main.cpp
    MainLoop.cpp
    MainLoop.hpp
    XiboApp.cpp
    XiboApp.hpp
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # TODO remove this workaround
target_link_libraries(${PROJECT_NAME}
    xmr
    cms
    networking
    schedule
    config
    control
    system
    dl
    backtrace
)

configure_file(ui.glade ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ui.glade COPYONLY)
configure_file(splash.jpg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/splash.jpg COPYONLY)
configure_file(snap_run.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snap_run.sh COPYONLY)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DESTINATION bin USE_SOURCE_PERMISSIONS)

enable_testing()

add_subdirectory(stat)
add_subdirectory(control)
add_subdirectory(cms)
add_subdirectory(schedule)
add_subdirectory(options)
add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(networking)
add_subdirectory(watchdog)
add_subdirectory(xmr)
