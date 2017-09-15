TEMPLATE = app
CONFIG += console c++14
CONFIG -= app_bundle
CONFIG -= qt

wxCXXFLAGS = $$system(wx-config --cxxflags --unicode=yes)
wxLinkOptions = $$system(wx-config --libs all --unicode=yes)
LIBS += $$wxLinkOptions
QMAKE_CXXFLAGS_RELEASE += $$wxCXXFLAGS
QMAKE_CXXFLAGS_DEBUG += $$wxCXXFLAGS

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
