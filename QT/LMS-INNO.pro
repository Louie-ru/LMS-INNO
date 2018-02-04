#-------------------------------------------------
#
# Project created by QtCreator 2018-01-19T15:02:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LMS-INNO
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += release
CONFIG += static
QMAKE_LFLAGS_RELEASE += -static -static-libgcc
QMAKE_LFLAGS += -static -static-libgcc
LIBS += -static-libgcc

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    patron.cpp \
    librarian.cpp \
    skeleton.cpp

HEADERS += \
        mainwindow.h \
    patron.h \
    librarian.h

FORMS += \
        mainwindow.ui \
    patron.ui \
    librarian.ui

RESOURCES +=
