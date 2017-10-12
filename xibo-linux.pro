TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle c++11
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)
thirdPartyCXXFLAGS = $$system(pkg-config --cflags gstreamer-video-1.0 gtk+-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gstreamer-video-1.0 gtk+-3.0)

LIBS += $$wxLinkOptions $$thirdPartyLinkOptions -lwx_gtk2u_media-3.1
QMAKE_CXXFLAGS += $$wxCXXFLAGS $$thirdPartyCXXFLAGS

SOURCES += main.cpp \
    PlayerApp.cpp \
    Layout.cpp \
    Media.cpp \
    VideoRender.cpp \
    Video.cpp \
    ImageRender.cpp \
    Region.cpp \
    Image.cpp

HEADERS += \
    PlayerApp.hpp \
    Layout.hpp \
    Media.hpp \
    VideoRender.hpp \
    Video.hpp \
    ImageRender.hpp \
    Region.hpp \
    Image.hpp
