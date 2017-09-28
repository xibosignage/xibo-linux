TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle c++11
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)

LIBS += $$wxLinkOptions -lwx_gtk2u_media-3.1
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS

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
