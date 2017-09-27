TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)
LIBS += $$wxLinkOptions  -lwx_gtk2u_media-3.1
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS

SOURCES += main.cpp \
    PlayerApp.cpp \
    Layout.cpp \
    ImageRender.cpp

HEADERS += \
    PlayerApp.hpp \
    Layout.hpp \
    ImageRender.hpp
