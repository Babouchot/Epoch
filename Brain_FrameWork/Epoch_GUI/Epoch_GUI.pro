#-------------------------------------------------
#
# Project created by QtCreator 2013-01-21T14:58:20
#
#-------------------------------------------------

QT       += core gui

TARGET = Epoch_GUI
TEMPLATE = app

LIBS += ../lib/libedk.so.1

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    ../Acquisition/fft.cpp \
    ../Acquisition/ArousalReader.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    ../Acquisition/Normalization.h \
    ../Acquisition/fft.h \
    ../Acquisition/EmoStateDLL.h \
    ../Acquisition/edkErrorCode.h \
    ../Acquisition/edk.h \
    ../Acquisition/ArousalReader.h \
    ../Acquisition/Algorithm.h

FORMS    += mainwindow.ui
