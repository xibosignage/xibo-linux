cmake_minimum_required(VERSION 3.14.1)

project(xibo-player)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

find_package(PkgConfig REQUIRED)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0>=3.22.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4>=2.56.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_TESTS_DIRECTORY ${CMAKE_BINARY_DIR}/tests)

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
add_compile_options(
    -pthread -Wall -Wno-parentheses -Wno-cast-function-type -W -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
    "$<$<CONFIG:RELEASE>:-O2>"
    "$<$<CONFIG:DEBUG>:-g3;-O0>"
)

add_compile_definitions(
    BOOST_THREAD_PROVIDES_FUTURE
    BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION
    BOOST_STACKTRACE_USE_ADDR2LINE
    BOOST_STACKTRACE_USE_BACKTRACE
)

set(APP_ENV "" CACHE STRING "Player environment (SNAP)")
if("${APP_ENV}" STREQUAL "SNAP")
    message("Build will be done for SNAP environment")
    set(SNAP_BUILD ON)
    add_compile_definitions(SNAP_ENABLED)
else()
    message("Build will be done for custom environment")
endif()

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

if(SNAP_BUILD)
    install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DESTINATION bin USE_SOURCE_PERMISSIONS)
    install(FILES resources/ui.glade resources/splash.jpg DESTINATION share/xibo-player)
    install(
        FILES snap_run.sh
        PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
        DESTINATION bin
    )
else()
    install(FILES resources/ui.glade resources/splash.jpg DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
endif()

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
