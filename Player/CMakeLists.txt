cmake_minimum_required(VERSION 3.8)

project(Player)

pkg_check_modules(GStreamer REQUIRED gstreamer-1.0>=1.8.0 gstreamer-video-1.0>=1.8.0 gstreamer-plugins-base-1.0>=1.8.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0>=2.4.10)

include_directories(${WebKit_INCLUDE_DIRS} ${GStreamer_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 -pthread -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,libs/")

set(SOURCE_FILES main.cpp XiboApp.cpp XlfParser.cpp MainLayout.cpp Media.cpp Image.cpp Video.cpp WebView.cpp Region.cpp Transition.cpp LayoutOverlay.cpp VideoHandler.cpp XiboVideoSink.cpp utilities.cpp)

find_link_libs("${GStreamer_LIBRARIES}" GStreamer_LINK_LIBRARIES)
find_link_libs("${WebKit_LIBRARIES}" WebKit_LINK_LIBRARIES)

find_soname_libs("${Gtkmm_LINK_LIBRARIES}" Gtkmm_SONAME_LIBRARIES)
find_soname_libs("${GStreamer_LINK_LIBRARIES}" GStreamer_SONAME_LIBRARIES)
find_soname_libs("${WebKit_LINK_LIBRARIES}" WebKit_SONAME_LIBRARIES)
find_soname_libs("${Boost_LIBRARIES}" Boost_SONAME_LIBRARIES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gcov ${Gtkmm_LINK_LIBRARIES} ${GStreamer_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES} ${Boost_LIBRARIES})

# install
install(DIRECTORY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/)
copy_libs("${Gtkmm_SONAME_LIBRARIES}")
copy_libs("${GStreamer_SONAME_LIBRARIES}")
copy_libs("${WebKit_SONAME_LIBRARIES}")
copy_libs("${Boost_SONAME_LIBRARIES}")

# Tests
include(PlayerTests)
