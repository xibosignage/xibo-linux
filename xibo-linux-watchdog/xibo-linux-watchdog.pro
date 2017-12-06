TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

#QMAKE_CXXFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_CFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_LFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak

thirdPartyCXXFLAGS = $$system(pkg-config --cflags gtkmm-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gtkmm-3.0)

LIBS += $$thirdPartyLinkOptions
QMAKE_CXXFLAGS += $$thirdPartyCXXFLAGS

SOURCES += main.cpp \
    MainWindow.cpp \
    WatchdogApp.cpp \
    PlayerProcessHandler.cpp \
    SystemTrayIcon.cpp

HEADERS += \
    WatchdogApp.hpp \
    MainWindow.hpp \
    PlayerProcessHandler.hpp \
    SystemTrayIcon.hpp
