QT += core dbus serialport
QT -= gui

TARGET = FlightComputerSerial
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    serial.cpp \
    RadioControlProtocol/rcLib.cpp

HEADERS += \
    serial.h \
    RadioControlProtocol/rcLib.hpp
