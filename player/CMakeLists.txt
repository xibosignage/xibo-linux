cmake_minimum_required(VERSION 3.14.1)

project(player)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

pkg_check_modules(GSTREAMER REQUIRED
    gstreamer-1.0>=1.16.2
    gstreamer-pbutils-1.0>=1.16.2
    gstreamer-plugins-base-1.0>=1.16.2
    gstreamer-video-1.0>=1.16.2)
pkg_check_modules(WEBKITGTK REQUIRED webkit2gtk-4.0>=2.28.4)
pkg_check_modules(GTKMM REQUIRED gtkmm-3.0>=3.22.0)
pkg_check_modules(GLIBMM REQUIRED glibmm-2.4>=2.56.0)
pkg_check_modules(X11 REQUIRED x11)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)
set(CMAKE_TESTS_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests/)

add_compile_options(
    -static-libstdc++ -pthread -Wall -Wno-parentheses -Wno-cast-function-type -W -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings
    "$<$<CONFIG:RELEASE>:-O2>"
    "$<$<CONFIG:DEBUG>:-g;-O0>"
)

add_compile_definitions(BOOST_THREAD_PROVIDES_FUTURE BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION USE_GTK)

option(SNAP_BUILD "Use SNAP environment during build" OFF)
if(SNAP_BUILD)
    add_compile_definitions(SNAP_ENABLED)
endif(SNAP_BUILD)

file(GLOB CONTROL_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/control/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/layout/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/region/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/media/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/media/image/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/media/player/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/media/player/audio/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/media/player/video/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/media/webview/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/transitions/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/widgets/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/control/widgets/gtk/*.*pp
)
file(GLOB OTHER_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/screenshot/*.*pp
    ${CMAKE_CURRENT_SOURCE_DIR}/stat/*.*pp
)
add_executable(${PROJECT_NAME}
    main.cpp
    MainLoop.cpp
    MainLoop.hpp
    XiboApp.cpp
    XiboApp.hpp
    ${OTHER_SOURCES}
    ${CONTROL_SOURCES}
)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}) # remove this workaround
target_link_libraries(${PROJECT_NAME}
    xmr
    cms
    networking
    schedule
    config
    ${X11_LINK_LIBRARIES}
    ${GTKMM_LINK_LIBRARIES}
    ${GSTREAMER_LINK_LIBRARIES}
    ${WEBKITGTK_LINK_LIBRARIES}
)
target_include_directories(${PROJECT_NAME} PRIVATE
    ${GTKMM_INCLUDE_DIRS}
    ${GSTREAMER_INCLUDE_DIRS}
    ${WEBKITGTK_INCLUDE_DIRS}
)

configure_file(ui.glade ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/ui.glade COPYONLY)
configure_file(splash.jpg ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/splash.jpg COPYONLY)
configure_file(snap_run.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/snap_run.sh COPYONLY)
install(DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} DESTINATION bin USE_SOURCE_PERMISSIONS)

enable_testing()

add_subdirectory(cms)
add_subdirectory(schedule)
add_subdirectory(options)
add_subdirectory(common)
add_subdirectory(config)
add_subdirectory(networking)
add_subdirectory(watchdog)
add_subdirectory(xmr)

