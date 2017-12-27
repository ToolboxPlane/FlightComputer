QT += core dbus serialport
QT -= gui

TARGET = FlightComputerSerial
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    serial.cpp

HEADERS += \
    serial.h
