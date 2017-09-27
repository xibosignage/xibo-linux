TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)

LIBS += $$wxLinkOptions -lwx_gtk2u_media-3.1
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS

SOURCES += main.cpp \
    MainWindow.cpp \
    PlayerApp.cpp \
    VideoRender.cpp

HEADERS += \
    MainWindow.hpp \
    PlayerApp.hpp \
    VideoRender.hpp
