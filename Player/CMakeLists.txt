cmake_minimum_required(VERSION 3.5.1)

project(Player)

pkg_check_modules(GStreamermm REQUIRED gstreamermm-1.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0)

include_directories(${Gtkmm_INCLUDE_DIRS} ${GStreamermm_INCLUDE_DIRS} ${WebKit_INCLUDE_DIRS})

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -pthread)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,libs/")

set(SOURCE_FILES main.cpp XiboApp.cpp LayoutOverlay.cpp Media.cpp Image.cpp Video.cpp WebView.cpp MainLayout.cpp Region.cpp VideoHandler.cpp XiboVideoSink.cpp Transition.cpp utilities.cpp XlfParser.cpp)

find_libs_path("${GStreamermm_LIBRARIES}" GStreamermm_FULL_LIBRARIES)
find_libs_path("${WebKit_LIBRARIES}" WebKit_FULL_LIBRARIES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${Gtkmm_FULL_LIBRARIES} ${GStreamermm_FULL_LIBRARIES} ${WebKit_FULL_LIBRARIES} ${Boost_LIBRARIES})

function(CopyLibs LibList)
    foreach(LibPath ${LibList})
        file(GLOB LibFullPath ${LibPath}.?)
        install(CODE "execute_process(COMMAND cp ${LibFullPath} ${CMAKE_CURRENT_BINARY_DIR}/libs/)")
    endforeach(LibPath)
endfunction(CopyLibs)

# install
install(DIRECTORY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/)
CopyLibs("${Gtkmm_FULL_LIBRARIES}")
CopyLibs("${GStreamermm_FULL_LIBRARIES}")
CopyLibs("${WebKit_FULL_LIBRARIES}")

# Tests
add_test(NAME player COMMAND ${PROJECT_NAME} --example-dir /home/stivius/XiboLinuxStack/XiboExamples/LayerTest --testing)
set_tests_properties(player PROPERTIES PASS_REGULAR_EXPRESSION "Player started")

add_test(NAME player-fake-dir COMMAND ${PROJECT_NAME} --example-dir /home/stivius/FakeDir --testing)
set_tests_properties(player-fake-dir PROPERTIES PASS_REGULAR_EXPRESSION "The directory doesn't exist")

add_test(NAME player-empty-dir COMMAND ${PROJECT_NAME} --example-dir /home --testing)
set_tests_properties(player-empty-dir PROPERTIES PASS_REGULAR_EXPRESSION ".XLF file doesn't exist")

add_test(NAME player-version COMMAND ${PROJECT_NAME} --version --testing)
set_tests_properties(player-version PROPERTIES PASS_REGULAR_EXPRESSION "Project version: ${PROJECT_VERSION}")

add_test(NAME player-wrong-command COMMAND ${PROJECT_NAME})
set_tests_properties(player-wrong-command PROPERTIES PASS_REGULAR_EXPRESSION "Allowed options")
