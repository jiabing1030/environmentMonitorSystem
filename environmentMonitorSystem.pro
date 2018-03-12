TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    talkWithSensorNet.c \
    talkwithdatacenter.c \
    talkWithAppServer.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    myglobal.h \
    talkWithSensorNet.h \
    talkwithdatacenter.h \
    talkWithAppServer.h

LIBS+=-lpthread



win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../mysql/lib/release/ -lmysqlclient
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../mysql/lib/debug/ -lmysqlclient
else:unix: LIBS += -L$$PWD/../../mysql/lib/ -lmysqlclient

INCLUDEPATH += $$PWD/../../mysql/include
DEPENDPATH += $$PWD/../../mysql/include
