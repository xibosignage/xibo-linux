cmake_minimum_required(VERSION 3.5.1)

project(Player)

pkg_check_modules(GStreamermm REQUIRED gstreamermm-1.0>=1.8.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0>=2.4.11)

include_directories(${GStreamermm_INCLUDE_DIRS} ${WebKit_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,libs/")

set(SOURCE_FILES main.cpp XiboApp.cpp LayoutOverlay.cpp Media.cpp Image.cpp Video.cpp WebView.cpp MainLayout.cpp Region.cpp VideoHandler.cpp XiboVideoSink.cpp Transition.cpp utilities.cpp XlfParser.cpp)

find_link_libs("${GStreamermm_LIBRARIES}" GStreamermm_LINK_LIBRARIES)
find_link_libs("${WebKit_LIBRARIES}" WebKit_LINK_LIBRARIES)

find_soname_libs("${Gtkmm_LINK_LIBRARIES}" Gtkmm_SONAME_LIBRARIES)
find_soname_libs("${GStreamermm_LINK_LIBRARIES}" GStreamermm_SONAME_LIBRARIES)
find_soname_libs("${WebKit_LINK_LIBRARIES}" WebKit_SONAME_LIBRARIES)
find_soname_libs("${Boost_LIBRARIES}" Boost_SONAME_LIBRARIES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${Gtkmm_LINK_LIBRARIES} ${GStreamermm_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES} ${Boost_LIBRARIES})

# install
install(DIRECTORY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/)
copy_libs("${Gtkmm_SONAME_LIBRARIES}")
copy_libs("${GStreamermm_SONAME_LIBRARIES}")
copy_libs("${WebKit_SONAME_LIBRARIES}")
copy_libs("${Boost_SONAME_LIBRARIES}")

# Tests
include(PlayerTests)
