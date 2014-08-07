#-------------------------------------------------
#
# Project created by QtCreator 2014-07-24T10:19:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Project
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h \
    ../Documents/sense-ml-new/image_impl.h \
    ../Documents/sense-ml-new/image.h

FORMS    += mainwindow.ui

RESOURCES += \
    Pics.qrc
