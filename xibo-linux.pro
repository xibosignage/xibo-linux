TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle c++11
CONFIG -= qt console

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)

thirdPartyCXXFLAGS = $$system(pkg-config --cflags webkitgtk-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs webkitgtk-3.0)

LIBS += $$wxLinkOptions $$thirdPartyLinkOptions
QMAKE_CXXFLAGS += $$wxCXXFLAGS $$thirdPartyCXXFLAGS -std=c++17

SOURCES += main.cpp \
    Media.cpp \
    VideoRender.cpp \
    Video.cpp \
    ImageRender.cpp \
    Region.cpp \
    Image.cpp \
    WebViewWrapper.cpp \
    XiboApp.cpp \
    EventsQueue.cpp \
    MainLayout.cpp

HEADERS += \
    Media.hpp \
    VideoRender.hpp \
    Video.hpp \
    ImageRender.hpp \
    Region.hpp \
    Image.hpp \
    WebViewWrapper.hpp \
    XiboApp.hpp \
    EventsQueue.hpp \
    MainLayout.hpp
