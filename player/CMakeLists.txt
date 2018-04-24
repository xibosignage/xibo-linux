cmake_minimum_required(VERSION 3.8)

project(player)

pkg_check_modules(GStreamer REQUIRED gstreamer-1.0>=1.8.0 gstreamer-video-1.0>=1.8.0 gstreamer-plugins-base-1.0>=1.8.0)
pkg_check_modules(WebKit REQUIRED webkitgtk-3.0>=2.4.10)

include_directories(${WebKit_INCLUDE_DIRS} ${GStreamer_INCLUDE_DIRS})
include_directories(${Gtkmm_INCLUDE_DIRS} ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -static-libstdc++ -O0 -pthread -Wall -W -Wshadow -Wunused-variable -Wunused-parameter -Wunused-function -Wunused -Wno-system-headers -Wno-deprecated -Woverloaded-virtual -Wwrite-strings")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,libs/")

set(CONTROL_SOURCES control/MainLayout.cpp control/Region.cpp control/LayoutOverlay.cpp)
set(MEDIA_SOURCES media/Media.cpp media/Image.cpp media/Video.cpp media/Audio.cpp media/WebView.cpp)
set(UTILS_SOURCES utils/utilities.cpp)
set(PARSER_SOURCES parsers/CommandLineParser.cpp parsers/LayoutParser.cpp parsers/RegionParser.cpp parsers/MediaParser.cpp)
set(CREATORS_SOURCES creators/LayoutBuilder.cpp creators/RegionBuilder.cpp creators/MediaFactory.cpp)
set(WRAPPER_SOURCES wrapper/Element.cpp wrapper/Caps.cpp wrapper/Pad.cpp wrapper/Message.cpp wrapper/Queue.cpp wrapper/Pipeline.cpp wrapper/Decodebin.cpp wrapper/FileSrc.cpp wrapper/Volume.cpp wrapper/VideoConvert.cpp wrapper/AudioConvert.cpp wrapper/VideoScale.cpp wrapper/AutoAudioSink.cpp)

set(SOURCE_FILES main.cpp XiboApp.cpp ${CONTROL_SOURCES} ${MEDIA_SOURCES} ${UTILS_SOURCES} ${PARSER_SOURCES} ${CREATORS_SOURCES})

add_library(gst_wrapper SHARED ${WRAPPER_SOURCES})
add_library(xibovideosink SHARED customsink/XiboVideoSink.cpp)

find_link_libs("${GStreamer_LIBRARIES}" GStreamer_LINK_LIBRARIES)
find_link_libs("${WebKit_LIBRARIES}" WebKit_LINK_LIBRARIES)

find_soname_libs("${Gtkmm_LINK_LIBRARIES}" Gtkmm_SONAME_LIBRARIES)
find_soname_libs("${GStreamer_LINK_LIBRARIES}" GStreamer_SONAME_LIBRARIES)
find_soname_libs("${WebKit_LINK_LIBRARIES}" WebKit_SONAME_LIBRARIES)
find_soname_libs("${Boost_LIBRARIES}" Boost_SONAME_LIBRARIES)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} gst_wrapper xibovideosink ${Gtkmm_LINK_LIBRARIES} ${GStreamer_LINK_LIBRARIES} ${WebKit_LINK_LIBRARIES} ${Boost_LIBRARIES})

# install
install(DIRECTORY DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/libs/)
copy_libs("${Gtkmm_SONAME_LIBRARIES}")
copy_libs("${GStreamer_SONAME_LIBRARIES}")
copy_libs("${WebKit_SONAME_LIBRARIES}")
copy_libs("${Boost_SONAME_LIBRARIES}")

# Tests
include(PlayerTests)
