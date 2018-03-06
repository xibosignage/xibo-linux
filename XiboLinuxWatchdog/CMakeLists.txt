cmake_minimum_required(VERSION 3.10)

project(XiboLinuxWatchdog)

find_package(PkgConfig REQUIRED)
find_package(Boost REQUIRED filesystem)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} ${GTKMM_CFLAGS_OTHER})
set(SOURCE_FILES main.cpp MainWindow.cpp PlayerProcessHandler.cpp SystemTrayIcon.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${GTKMM_LIBRARIES} ${Boost_LIBRARIES})
