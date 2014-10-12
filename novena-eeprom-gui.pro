#-------------------------------------------------
#
# Project created by QtCreator 2014-07-17T19:05:49
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = novena-eeprom-gui
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    EepromDev.cpp

HEADERS  += mainwindow.h \
    EepromDev.h

FORMS    += mainwindow.ui

OTHER_FILES += \
    novena_logo_web.png

RESOURCES += \
    logo.qrc
