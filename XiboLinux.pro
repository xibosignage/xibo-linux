TEMPLATE = app
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt console

#QMAKE_CXXFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_CFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_LFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak

thirdPartyCXXFLAGS = $$system(pkg-config --cflags gtkmm-3.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-base-1.0 webkitgtk-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gtkmm-3.0 gstreamer-1.0 gstreamer-video-1.0 gstreamer-base-1.0 webkitgtk-3.0)

LIBS += $$thirdPartyLinkOptions -lpthread
QMAKE_CXXFLAGS += $$thirdPartyCXXFLAGS -Wno-deprecated -pthread -O2

SOURCES += main.cpp \
    MainLayout.cpp \
    Region.cpp \
    Media.cpp \
    Image.cpp \
    Video.cpp \
    WebView.cpp \
    VideoHandler.cpp

HEADERS += \
    MainLayout.hpp \
    Region.hpp \
    Media.hpp \
    Image.hpp \
    Video.hpp \
    WebView.hpp \
    VideoHandler.hpp \
    BindWrapper.hpp \
    constants.hpp
