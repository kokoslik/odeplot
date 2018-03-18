#-------------------------------------------------
#
# Project created by QtCreator 2018-03-15T19:35:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = odeplot
TEMPLATE = app

CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui


INCLUDEPATH += /usr/include/boost
