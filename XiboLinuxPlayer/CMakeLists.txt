cmake_minimum_required(VERSION 3.10)

project(XiboLinuxPlayer)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED filesystem program_options)

pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0)
pkg_check_modules(GStreamermm REQUIRED gstreamermm-1.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0)

include_directories(${Gtkmm_INCLUDE_DIRS} ${GStreamermm_INCLUDE_DIRS} ${WebKit_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(SOURCE_FILES main.cpp XiboApp.cpp LayoutOverlay.cpp Media.cpp Image.cpp Video.cpp WebView.cpp MainLayout.cpp Region.cpp
                          VideoHandler.cpp XiboVideoSink.cpp Transition.cpp utilities.cpp XlfParser.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${Gtkmm_LIBRARIES} ${GStreamermm_LIBRARIES} ${WebKit_LIBRARIES} ${Boost_LIBRARIES})
