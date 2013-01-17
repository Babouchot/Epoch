#-------------------------------------------------
#
# Project created by QtCreator 2011-04-18T09:44:27
#
#-------------------------------------------------

QT       += core

QT       -= gui
#DESTDIR =/emotiv/
TARGET = example5


TEMPLATE = app


SOURCES += main.cpp

HEADERS += \
    EmoStateDLL.h \
    edkErrorCode.h \
    edk.h

LIBS+= -L$$/root/EmotivDeveloper_1.0.0.5/lib -ledk -ledk_utils
