project(player)

pkg_check_modules(GStreamer REQUIRED gstreamer-1.0>=1.8.0 gstreamer-pbutils-1.0 gstreamer-video-1.0>=1.8.0 gstreamer-plugins-base-1.0>=1.8.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0>=2.4.10)
pkg_check_modules(Gtkmm REQUIRED gtkmm-3.0>=3.18.0)

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${Gtkmm_INCLUDE_DIRS} ${GStreamer_INCLUDE_DIRS}  ${WebKit_INCLUDE_DIRS})

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,libs/")

add_compile_definitions(BOOST_THREAD_PROVIDES_FUTURE BOOST_THREAD_PROVIDES_FUTURE_CONTINUATION)

file(GLOB MAIN_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*.*pp)
file(GLOB_RECURSE PLAYER_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/control/*.*pp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/utils/*.*pp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/managers/*.*pp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/gstsink/*.*pp
                                 ${CMAKE_CURRENT_SOURCE_DIR}/xmlsink/*.*pp)

add_subdirectory(options)
add_subdirectory(common)
add_subdirectory(gstwrapper)
add_subdirectory(networking)
add_subdirectory(watchdog)
#add_subdirectory(xmr)

add_executable(${PROJECT_NAME} ${MAIN_SOURCES} ${PLAYER_SOURCES})
target_link_libraries(${PROJECT_NAME} X11 gstwrapper common ${Gtkmm_LINK_LIBRARIES} ${GStreamer_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES} ${Boost_LIBRARIES})

copy_libs("${Gtkmm_LINK_LIBRARIES}")
copy_libs("${GStreamer_LINK_LIBRARIES}")
copy_libs("${WebKit_LINK_LIBRARIES}")
copy_libs("${Boost_LIBRARIES}")
