project(player)

pkg_check_modules(GStreamer REQUIRED gstreamermm-1.0>=1.10.0 gstreamer-pbutils-1.0 gstreamer-video-1.0>=1.14.0 gstreamer-plugins-base-1.0>=1.14.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0>=2.4.10)
pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0>=3.18.0)
pkg_check_modules(Glibmm REQUIRED glibmm-2.4>=2.56.0)
pkg_check_modules(Spdlog REQUIRED spdlog>=1.4.1)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Gtkmm_INCLUDE_DIRS} ${GStreamer_INCLUDE_DIRS}  ${WebKit_INCLUDE_DIRS})

add_compile_definitions(BOOST_THREAD_PROVIDES_FUTURE BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION)

file(GLOB MAIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)
file(GLOB_RECURSE CONTROL_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/control/*.*pp)
file(GLOB OTHER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/screenshot/*.*pp
                         ${CMAKE_CURRENT_SOURCE_DIR}/managers/*.*pp)

add_subdirectory(schedule)
add_subdirectory(options)
add_subdirectory(common)
add_subdirectory(networking)
add_subdirectory(watchdog)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${OTHER_SOURCES} ${CONTROL_SOURCES})
target_link_libraries(${PROJECT_NAME} X11 networking schedule ${Gtkmm_LINK_LIBRARIES} ${GStreamer_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES})

copy_libs("${Boost_LIBRARIES}")
