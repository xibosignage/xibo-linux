TEMPLATE = app
CONFIG += c++14 console
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CXXFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_CFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_LFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak

thirdPartyCXXFLAGS = $$system(pkg-config --cflags gtkmm-3.0 gstreamermm-1.0 gstreamer-base-1.0 webkitgtk-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gtkmm-3.0 gstreamermm-1.0  gstreamer-base-1.0 webkitgtk-3.0)

LIBS += $$thirdPartyLinkOptions -L../boost_1_66_0/stage/lib -lpthread -lboost_system -lboost_filesystem
QMAKE_CXXFLAGS += $$thirdPartyCXXFLAGS -Wno-deprecated -pthread -O2

INCLUDEPATH += ../boost_1_66_0

SOURCES += main.cpp \
    MainLayout.cpp \
    WebView.cpp \
    VideoHandler.cpp \
    RegionParser.cpp \
    Media.cpp \
    Video.cpp \
    Transition.cpp \
    Image.cpp \
    MainParser.cpp \
    ImageParser.cpp \
    VideoParser.cpp \
    Region.cpp \
    WebViewParser.cpp \
    LayoutOverlay.cpp \
    XiboVideoSink.cpp

HEADERS += \
    MainLayout.hpp \
    Media.hpp \
    Image.hpp \
    Video.hpp \
    WebView.hpp \
    VideoHandler.hpp \
    constants.hpp \
    RegionParser.hpp \
    MediaParser.hpp \
    Transition.hpp \
    Image.hpp \
    MainParser.hpp \
    ImageParser.hpp \
    VideoParser.hpp \
    Region.hpp \
    WebViewParser.hpp \
    LayoutOverlay.hpp \
    XiboVideoSink.hpp
