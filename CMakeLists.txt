cmake_minimum_required(VERSION 3.10)

project(XiboLinux)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(XiboLinuxPlayer)
add_subdirectory(XiboLinuxWatchdog)
