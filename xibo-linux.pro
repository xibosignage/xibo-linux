TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)

LIBS += $$wxLinkOptions
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS

INCLUDEPATH += ../boost_1_65_1

SOURCES += main.cpp \
    RegionParser.cpp \
    LayoutParser.cpp \
    Layout.cpp \
    Region.cpp \
    Media.cpp \
    Video.cpp \
    MediaParser.cpp \
    Transition.cpp \
    Image.cpp

HEADERS += \
    Parser.hpp \
    RegionParser.hpp \
    LayoutParser.hpp \
    Layout.hpp \
    Region.hpp \
    Media.hpp \
    Video.hpp \
    MediaParser.hpp \
    Transition.hpp \
    utilities.hpp \
    Image.hpp
