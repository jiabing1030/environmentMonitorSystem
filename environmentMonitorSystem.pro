TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.c \
    talkWithSensorNet.c \
    talkwithdatacenter.c

include(deployment.pri)
qtcAddDeployment()

HEADERS += \
    myglobal.h \
    talkWithSensorNet.h \
    talkwithdatacenter.h

LIBS+=-lpthread


