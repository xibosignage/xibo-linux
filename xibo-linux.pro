TEMPLATE = app
#CONFIG += console
CONFIG -= app_bundle c++11
CONFIG -= qt console

thirdPartyCXXFLAGS = $$system(pkg-config --cflags gtkmm-3.0 gstreamer-video-1.0 webkitgtk-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gtkmm-3.0 gstreamer-video-1.0 webkitgtk-3.0)

LIBS += $$thirdPartyLinkOptions
QMAKE_CXXFLAGS += $$thirdPartyCXXFLAGS -std=c++14

SOURCES += main.cpp \
    MainLayout.cpp \
    Region.cpp \
    Media.cpp \
    Image.cpp \
    Video.cpp \
    WebView.cpp

HEADERS += \
    MainLayout.hpp \
    Region.hpp \
    Media.hpp \
    Image.hpp \
    Video.hpp \
    WebView.hpp
