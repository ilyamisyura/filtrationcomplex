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
    qcustomplot.cpp \
    signalprocessor.cpp \
    detailsform.cpp \
    datasignal.cpp \
    qcustomplotgraph.cpp \
    abstractgenerator.cpp \
    discretegenerator.cpp \
    discreteprocessor.cpp

HEADERS  += mainwindow.h \
    filtercore.h \
    signalgenerator.h \
    qcustomplot.h \
    signalprocessor.h \
    detailsform.h \
    datasignal.h \
    qcustomplotgraph.h \
    abstractgenerator.h \
    discretegenerator.h \
    discreteprocessor.h

FORMS    += mainwindow.ui \
    detailsform.ui

CONFIG+=c++11
