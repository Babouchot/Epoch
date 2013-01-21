#-------------------------------------------------
#
# Project created by QtCreator 2013-01-21T14:58:20
#
#-------------------------------------------------

QT       += core gui

TARGET = Epoch_GUI
TEMPLATE = app

LIBS += /home/maureill/Bureau/Projet_Epoch/Epoch/Brain_FrameWork/Epoch_GUI/libedk.so.1

SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    fft.cpp \
    ArousalReader.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    Normalization.h \
    fft.h \
    EmoStateDLL.h \
    edkErrorCode.h \
    edk.h \
    ArousalReader.h \
    Algorithm.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    libedk_utils.so \
    libedk.so.1
