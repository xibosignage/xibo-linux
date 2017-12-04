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
    Layout.cpp \
    Region.cpp \
    Media.cpp \
    Video.cpp \
    Transition.cpp \
    Image.cpp \
    MainParser.cpp \
    ImageParser.cpp \
    VideoParser.cpp

HEADERS += \
    Parser.hpp \
    RegionParser.hpp \
    Layout.hpp \
    Region.hpp \
    Media.hpp \
    Video.hpp \
    MediaParser.hpp \
    Transition.hpp \
    utilities.hpp \
    Image.hpp \
    MainParser.hpp \
    ImageParser.hpp \
    VideoParser.hpp
