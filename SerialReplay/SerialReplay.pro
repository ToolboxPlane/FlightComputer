QT += core dbus
QT -= gui

TARGET = SerialReplay
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

SOURCES += main.cpp

DISTFILES += \
    test.rec

