cmake_minimum_required(VERSION 3.10)

project(XiboLinuxCMake)

message("PKG_CONFIG_FOUND ${PKG_CONFIG_FOUND}")
message("PKG_CONFIG_EXECUTABLE ${PKG_CONFIG_EXECUTABLE}")
message("PKG_CONFIG_VERSION_STRING ${PKG_CONFIG_VERSION_STRING}")

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED filesystem program_options)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)
pkg_check_modules(GSTREAMERMM REQUIRED gstreamermm-1.0)
pkg_check_modules(WEBKIT REQUIRED webkitgtk-3.0)

include_directories(${GTKMM_INCLUDE_DIRS} ${GSTREAMERMM_INCLUDE_DIRS} ${WEBKIT_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GTKMM_CFLAGS_OTHER})
set(SOURCE_FILES main.cpp XiboApp.cpp LayoutOverlay.cpp Media.cpp Image.cpp Video.cpp WebView.cpp MainLayout.cpp Region.cpp
                          VideoHandler.cpp XiboVideoSink.cpp Transition.cpp utilities.cpp XlfParser.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} ${GSTREAMERMM_LIBRARIES} ${WEBKIT_LIBRARIES} ${Boost_LIBRARIES})
