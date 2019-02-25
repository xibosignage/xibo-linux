cmake_minimum_required(VERSION 3.8)

project(player)

pkg_check_modules(GStreamer REQUIRED gstreamer-1.0>=1.8.0 gstreamer-pbutils-1.0 gstreamer-video-1.0>=1.8.0 gstreamer-plugins-base-1.0>=1.8.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0>=2.4.10)
pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0>=3.18.0)

include_directories(${WebKit_INCLUDE_DIRS} ${GStreamer_INCLUDE_DIRS})
include_directories(${Gtkmm_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -O0 -pthread -Wall -Wno-parentheses -Wno-cast-function-type -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,libs/")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage -g")
set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fprofile-arcs -ftest-coverage")

add_compile_definitions(BOOST_THREAD_PROVIDES_FUTURE BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION)

file(GLOB ADAPTORS_SOURCES ${CMAKE_SOURCE_DIR}/player/adaptors/*.cpp ${CMAKE_SOURCE_DIR}/player/adaptors/*.hpp)
file(GLOB CONTROL_SOURCES ${CMAKE_SOURCE_DIR}/player/control/*.cpp ${CMAKE_SOURCE_DIR}/player/control/*.hpp)
file(GLOB MEDIA_SOURCES ${CMAKE_SOURCE_DIR}/player/media/*.cpp ${CMAKE_SOURCE_DIR}/player/media/*.hpp)
file(GLOB UTILS_SOURCES ${CMAKE_SOURCE_DIR}/player/utils/*.cpp ${CMAKE_SOURCE_DIR}/player/utils/*.hpp)
file(GLOB CREATORS_SOURCES ${CMAKE_SOURCE_DIR}/player/creators/*.cpp ${CMAKE_SOURCE_DIR}/player/creators/*.hpp)
file(GLOB MANAGERS_SOURCES ${CMAKE_SOURCE_DIR}/player/managers/*.cpp ${CMAKE_SOURCE_DIR}/player/managers/*.hpp)
file(GLOB PARSERS_SOURCES ${CMAKE_SOURCE_DIR}/player/options/*.cpp ${CMAKE_SOURCE_DIR}/player/options/*.hpp)
file(GLOB WRAPPER_SOURCES ${CMAKE_SOURCE_DIR}/player/wrapper/*.cpp ${CMAKE_SOURCE_DIR}/player/wrapper/*.hpp)
file(GLOB XMDS_SOURCES ${CMAKE_SOURCE_DIR}/player/xmds/*.cpp ${CMAKE_SOURCE_DIR}/player/xmds/*.hpp)
file(GLOB EVENTS_SOURCES ${CMAKE_SOURCE_DIR}/player/events/*.cpp ${CMAKE_SOURCE_DIR}/player/events/*.hpp)

add_library(player_lib STATIC MainLoop.hpp MainLoop.cpp XiboApp.cpp
                              ${ADAPTORS_SOURCES} ${CONTROL_SOURCES} ${MEDIA_SOURCES}
                              ${UTILS_SOURCES} ${CREATORS_SOURCES} ${MANAGERS_SOURCES}
                              ${PARSERS_SOURCES} ${XMDS_SOURCES} ${EVENTS_SOURCES})
target_link_libraries(player_lib stdc++fs ssl crypto)
add_library(gst_wrapper SHARED ${WRAPPER_SOURCES})
add_library(xibovideosink SHARED customsink/XiboVideoSink.cpp)

find_link_libs("${GStreamer_LIBRARIES}" GStreamer_LINK_LIBRARIES)
find_link_libs("${WebKit_LIBRARIES}" WebKit_LINK_LIBRARIES)
find_link_libs("${Gtkmm_LIBRARIES}" Gtkmm_LINK_LIBRARIES)

find_soname_libs("${Gtkmm_LINK_LIBRARIES}" Gtkmm_SONAME_LIBRARIES)
find_soname_libs("${GStreamer_LINK_LIBRARIES}" GStreamer_SONAME_LIBRARIES)
find_soname_libs("${WebKit_LINK_LIBRARIES}" WebKit_SONAME_LIBRARIES)
find_soname_libs("${Boost_LIBRARIES}" Boost_SONAME_LIBRARIES)

# Tests
add_subdirectory(tests)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} player_lib gst_wrapper xibovideosink ${Gtkmm_LINK_LIBRARIES} ${GStreamer_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES} ${Boost_LIBRARIES})


# install
install(DIRECTORY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/)
copy_libs("${Gtkmm_SONAME_LIBRARIES}")
copy_libs("${GStreamer_SONAME_LIBRARIES}")
copy_libs("${WebKit_SONAME_LIBRARIES}")
copy_libs("${Boost_SONAME_LIBRARIES}")
copy_libs("${CMAKE_CURRENT_BINARY_DIR}/libgst_wrapper.so ${CMAKE_CURRENT_BINARY_DIR}/libxibovideosink.so")
