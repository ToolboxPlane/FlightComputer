QT += core
QT -= gui

TARGET = LoRaDbus
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

QMAKE_CFLAGS        = -DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY -D__BASEFILE__=\"$*\"
LIBS          = -lbcm2835
RADIOHEADBASE = /home/pi/RadioHead/
INCLUDEPATH       = $(RADIOHEADBASE)

SOURCES += main.cpp \
    lorahal.cpp \
    RadioControlProtocol/rcLib.cpp \
$$RADIOHEADBASE/RHutil/RasPi.cpp \
$$RADIOHEADBASE/RH_RF95.cpp \
$$RADIOHEADBASE/RHDatagram.cpp \
$$RADIOHEADBASE/RHHardwareSPI.cpp \
$$RADIOHEADBASE/RHSPIDriver.cpp \
$$RADIOHEADBASE/RHGenericDriver.cpp \
$$RADIOHEADBASE/RHGenericSPI.cpp

HEADERS += \
    lorahal.h \
    RadioControlProtocol/rcLib.hpp \
$$RADIOHEADBASE/RHutil/RasPi.hpp \
$$RADIOHEADBASE/RH_RF95.hpp \
$$RADIOHEADBASE/RHDatagram.hpp \
$$RADIOHEADBASE/RHHardwareSPI.hpp \
$$RADIOHEADBASE/RHSPIDriver.hpp \
$$RADIOHEADBASE/RHGenericDriver.hpp \
$$RADIOHEADBASE/RHGenericSPI.hpp

