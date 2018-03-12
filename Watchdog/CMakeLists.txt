cmake_minimum_required(VERSION 3.5.1)

project(Watchdog)

pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0)

include_directories(${Gtkmm_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(SOURCE_FILES main.cpp MainWindow.cpp PlayerProcessHandler.cpp SystemTrayIcon.cpp)

#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} ${Gtkmm_FULL_LIBRARIES} ${Boost_LIBRARIES})
