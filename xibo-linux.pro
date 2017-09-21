TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle c++11
CONFIG -= qt

QMAKE_CC = gcc-7
QMAKE_CXX = g++-7

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)
LIBS += $$wxLinkOptions
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS -std=gnu++17
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS -std=gnu++17

SOURCES += main.cpp \
    MainWindow.cpp \
    PlayerApp.cpp \
    Parser.cpp \
    RegionParser.cpp \
    LayoutParser.cpp \
    Layout.cpp \
    Region.cpp \
    Media.cpp \
    Video.cpp \
    MediaParser.cpp \
    MediaFactory.cpp \
    Transition.cpp \
    Image.cpp

HEADERS += \
    MainWindow.hpp \
    PlayerApp.hpp \
    Parser.hpp \
    RegionParser.hpp \
    LayoutParser.hpp \
    Layout.hpp \
    Region.hpp \
    Media.hpp \
    Video.hpp \
    XiboObject.hpp \
    MediaParser.hpp \
    MediaFactory.hpp \
    Transition.hpp \
    utilities.hpp \
    Image.hpp
