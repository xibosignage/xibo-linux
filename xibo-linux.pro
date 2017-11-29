TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle c++11
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)

LIBS += $$wxLinkOptions
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS -std=gnu++17
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS -std=gnu++17

INCLUDEPATH += ../boost_1_65_1

SOURCES += main.cpp \
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
    Image.cpp \
    utilities.cpp

HEADERS += \
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
    Image.hpp \
    constants.hpp
