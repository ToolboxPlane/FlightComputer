QT += core dbus
QT -= gui

TARGET = SerialReplay
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp

DISTFILES += \
    testrecording

