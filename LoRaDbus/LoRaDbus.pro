QT += core
QT -= gui

TARGET = LoRaDbus
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    lorahal.cpp \
    RadioControlProtocol/rcLib.cpp

HEADERS += \
    lorahal.h \
    RadioControlProtocol/rcLib.hpp

