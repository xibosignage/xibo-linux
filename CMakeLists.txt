cmake_minimum_required(VERSION 3.10)

execute_process(COMMAND git rev-list --all --count OUTPUT_VARIABLE GIT_REVISION)
string(REGEX REPLACE "\n$" "" GIT_REVISION "${GIT_REVISION}")

project(XiboLinux VERSION 0.2.0.${GIT_REVISION})

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)

if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config was not found")
endif()

if(UBUNTU_16_04)
    find_package(Boost 1.58 REQUIRED filesystem program_options)
    message(WARNING "Using old package")
else()
    find_package(Boost 1.66 REQUIRED filesystem program_options)
endif()

include(CTest)

add_test(NAME player COMMAND XiboLinuxPlayer --example-dir /home/stivius/XiboLinuxStack/XiboExamples/LayerTest --testing)
set_tests_properties(player PROPERTIES PASS_REGULAR_EXPRESSION "Player started")

add_test(NAME player-fake-dir COMMAND XiboLinuxPlayer --example-dir /home/stivius/FakeDir --testing)
set_tests_properties(player-fake-dir PROPERTIES PASS_REGULAR_EXPRESSION "The directory doesn't exist")

add_test(NAME player-empty-dir COMMAND XiboLinuxPlayer --example-dir /home/stivius/XiboLinuxStack/XiboExamples/FakeEmptyDir --testing)
set_tests_properties(player-empty-dir PROPERTIES PASS_REGULAR_EXPRESSION ".XLF file doesn't exist")

add_test(NAME player-version COMMAND XiboLinuxPlayer --version --testing)
set_tests_properties(player-version PROPERTIES PASS_REGULAR_EXPRESSION "Project version: ${PROJECT_VERSION}")

add_test(NAME player-wrong-command COMMAND XiboLinuxPlayer)
set_tests_properties(player-wrong-command PROPERTIES PASS_REGULAR_EXPRESSION "Allowed options")

configure_file(config.hpp.in ${CMAKE_BINARY_DIR}/generated/config.hpp)
include_directories(${CMAKE_BINARY_DIR}/generated/)

add_subdirectory(XiboLinuxPlayer)
add_subdirectory(XiboLinuxWatchdog)
