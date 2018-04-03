cmake_minimum_required(VERSION 3.5.1)

project(Watchdog)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(SOURCE_FILES main.cpp MainWindow.cpp PlayerProcessHandler.cpp SystemTrayIcon.cpp)

#add_executable(${PROJECT_NAME} ${SOURCE_FILES})
#target_link_libraries(${PROJECT_NAME} ${Gtkmm_FULL_LIBRARIES} ${Boost_LIBRARIES})
