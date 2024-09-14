#-------------------------------------------------
#
# Project created by QtCreator 2024-09-13T11:29:54
#
#-------------------------------------------------

QT       += core gui serialport serialbus

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = SerialProtTool
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR += $$PWD/bin_vs

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
RC_FILE += myico.rc

SOURCES += \
        main.cpp \
        mainwindow.cpp \
    serialportwrap.cpp \
    widgetutils.cpp \
    configvar.cpp \
    qtabout.cpp

HEADERS += \
        mainwindow.h \
    serialportwrap.h \
    logger.h \
    widgetutils.h \
    configvar.h \
    qtabout.h \
    singleton.h

FORMS += \
        mainwindow.ui \
    qtabout.ui

DISTFILES += \
    myico.rc

RESOURCES += \
    source.qrc
