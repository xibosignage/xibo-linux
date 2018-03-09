TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt c++14

#QMAKE_CXXFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_CFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_LFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak

thirdPartyCXXFLAGS = $$system(pkg-config --cflags gtkmm-3.0 gstreamermm-1.0 gstreamer-base-1.0 webkitgtk-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gtkmm-3.0 gstreamermm-1.0  gstreamer-base-1.0 webkitgtk-3.0)

LIBS += $$thirdPartyLinkOptions -L../boost_1_66_0/stage/lib -lpthread -lboost_system -lboost_filesystem -lboost_program_options
QMAKE_CXXFLAGS += $$thirdPartyCXXFLAGS -Wno-deprecated -pthread -O2 -std=c++14
QMAKE_LFLAGS += -Wl,-rpath,libs

INCLUDEPATH += ../boost_1_66_0

SOURCES += main.cpp \
    MainLayout.cpp \
    WebView.cpp \
    VideoHandler.cpp \
    Media.cpp \
    Video.cpp \
    Transition.cpp \
    Image.cpp \
    Region.cpp \
    LayoutOverlay.cpp \
    XiboVideoSink.cpp \
    XiboApp.cpp \
    XlfParser.cpp \
    utilities.cpp

HEADERS += \
    MainLayout.hpp \
    Media.hpp \
    Image.hpp \
    Video.hpp \
    WebView.hpp \
    VideoHandler.hpp \
    constants.hpp \
    Transition.hpp \
    Image.hpp \
    Region.hpp \
    LayoutOverlay.hpp \
    XiboVideoSink.hpp \
    XiboApp.hpp \
    XlfParser.hpp \
    utilities.hpp
