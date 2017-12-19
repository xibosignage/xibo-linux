TEMPLATE = app
CONFIG += c++14
CONFIG -= app_bundle
CONFIG -= qt console

#QMAKE_CXXFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_CFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak
#QMAKE_LFLAGS+= -fsanitize=address -fsanitize=undefined -fsanitize=leak

thirdPartyCXXFLAGS = $$system(pkg-config --cflags gtkmm-3.0)
thirdPartyLinkOptions = $$system(pkg-config --libs gtkmm-3.0)

LIBS += $$thirdPartyLinkOptions -L../../boost_1_65_1/stage/lib -lpthread -lboost_system -lboost_filesystem
QMAKE_CXXFLAGS += $$thirdPartyCXXFLAGS -Wno-deprecated -pthread -O2

INCLUDEPATH += ../../boost_1_65_1

SOURCES += main.cpp \
    MainWindow.cpp \
    PlayerProcessHandler.cpp \
    SystemTrayIcon.cpp

HEADERS += \
    MainWindow.hpp \
    PlayerProcessHandler.hpp \
    SystemTrayIcon.hpp \
    Dispatcher.hpp
