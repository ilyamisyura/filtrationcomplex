#-------------------------------------------------
#
# Project created by QtCreator 2014-12-22T17:08:48
#
#-------------------------------------------------

QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = FiltrationComplex
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    filtercore.cpp \
    signalgenerator.cpp \
    qcustomplot.cpp

HEADERS  += mainwindow.h \
    filtercore.h \
    signalgenerator.h \
    qcustomplot.h

FORMS    += mainwindow.ui

CONFIG+=c++11