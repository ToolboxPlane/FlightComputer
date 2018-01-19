QT += core dbus
QT -= gui

TARGET = LoRaDbus
CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TEMPLATE = app

QMAKE_CXXFLAGS += -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY #-D__BASEFILE__=\"$*\"
LIBS         += -lbcm2835
RADIOHEADBASE = /home/pi/RadioHead/
INCLUDEPATH  += $$RADIOHEADBASE

SOURCES += main.cpp \
    LoRaHal.cpp \
    RadioControlProtocol/rcLib.cpp \
$$RADIOHEADBASE/RHutil/RasPi.cpp \
$$RADIOHEADBASE/RH_RF95.cpp \
$$RADIOHEADBASE/RHDatagram.cpp \
$$RADIOHEADBASE/RHHardwareSPI.cpp \
$$RADIOHEADBASE/RHSPIDriver.cpp \
$$RADIOHEADBASE/RHGenericDriver.cpp \
$$RADIOHEADBASE/RHGenericSPI.cpp

HEADERS += \
    LoRaHal.h \
    RadioControlProtocol/rcLib.hpp  \
$$RADIOHEADBASE/RHutil/RasPi.h \
$$RADIOHEADBASE/RH_RF95.h \
$$RADIOHEADBASE/RHDatagram.h \
$$RADIOHEADBASE/RHHardwareSPI.h \
$$RADIOHEADBASE/RHSPIDriver.h \
$$RADIOHEADBASE/RHGenericDriver.h \
$$RADIOHEADBASE/RHGenericSPI.h

