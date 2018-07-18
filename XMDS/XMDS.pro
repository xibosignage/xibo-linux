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
    Requests/GetFileRequest.cpp \
    Requests/NotifyStatusRequest.cpp \
    Requests/RegisterDisplayRequest.cpp \
    Requests/SubmitLogRequest.cpp \
    Requests/SubmitStatsRequest.cpp \
    Responses/GetFileResponse.cpp \
    Responses/RegisterDisplayResponse.cpp \
    Responses/RequiredFilesResponse.cpp \
    Responses/ScheduleResponse.cpp \
    main.cpp \
    utils.cpp

HEADERS += \
    Requests/BaseRequest.hpp \
    Requests/GetFileRequest.hpp \
    Requests/NotifyStatusRequest.hpp \
    Requests/RegisterDisplayRequest.hpp \
    Requests/RequiredFilesRequest.hpp \
    Requests/ScheduleRequest.hpp \
    Requests/SubmitLogRequest.hpp \
    Requests/SubmitStatsRequest.hpp \
    Responses/BaseResponse.hpp \
    Responses/GetFileResponse.hpp \
    Responses/RegisterDisplayResponse.hpp \
    Responses/RequiredFilesResponse.hpp \
    Responses/ResponsesFwd.hpp \
    Responses/ScheduleResponse.hpp \
    Responses/SuccessResponse.hpp \
    Responses/XmlResponse.hpp \
    constants.hpp \
    utils.hpp
