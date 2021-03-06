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

#Lines for Nikolai only
#CONFIG += release
#CONFIG += static
#QMAKE_LFLAGS_RELEASE += -static -static-libgcc
#QMAKE_LFLAGS += -static -static-libgcc
#LIBS += -static-libgcc
#end

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    patron.cpp \
    librarian.cpp \
    skeleton.cpp \
    hasher.cpp \
    sha_512.cpp \
    admin.cpp \
    queue.cpp \
    tester.cpp

HEADERS += \
        mainwindow.h \
    patron.h \
    librarian.h \
    hasher.h \
    sha_512.h \
    admin.h \
    queue.h \
    tester.h

FORMS += \
        mainwindow.ui \
    patron.ui \
    librarian.ui \
    admin.ui

RESOURCES += \
    logo_name.qrc

#probably another command for linux/mac
win32:RC_ICONS += logo_book.ico

OTHER_FILES +=
