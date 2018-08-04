QT -= gui

CONFIG += console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
QMAKE_CXXFLAGS += -std=c++17

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

LIBS += -lssl -lcrypto -L/usr/local/lib -lboost_system -lboost_thread

SOURCES += \
    main.cpp \
    soap.cpp \
    RequiredFiles.cpp \
    GetFile.cpp \
    MediaInventory.cpp \
    NotifyStatus.cpp \
    RegisterDisplay.cpp \
    SubmitLog.cpp \
    SubmitStats.cpp \
    utils.cpp \
    xmds.cpp

HEADERS += \
    soap.hpp \
    RegisterDisplay.hpp \
    field.hpp \
    RequiredFiles.hpp \
    SubmitLog.hpp \
    SubmitStats.hpp \
    NotifyStatus.hpp \
    MediaInventory.hpp \
    GetFile.hpp \
    utils.hpp \
    xmds.hpp
