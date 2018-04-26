cmake_minimum_required(VERSION 3.5)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        main.cpp
        ChannelMultiplexer.hpp
        Channel.hpp
        ThreadModule.hpp
        Devices/FlightController/FlightController.cpp
        Devices/FlightController/FlightController.hpp
        Devices/rcLib/RadioControlProtocol/rcLib.cpp
        Devices/rcLib/RadioControlProtocol/rcLib.hpp
        Devices/rcLib/PackageExtended.hpp
        Devices/rcLib/RcLibSimulator.cpp
        Devices/rcLib/RcLibSimulator.hpp
        Devices/GPS/Gps_t.hpp
        Devices/Serial.cpp
        Devices/Serial.hpp
        Filters/Fusion/Fusion.cpp
        Filters/Fusion/Fusion.hpp
        Filters/Navigation/Navigation.cpp
        Filters/Navigation/Navigation.hpp
        Filters/MeshManager/MeshManager.cpp
        Filters/MeshManager/MeshManager.hpp
        Filters/Navigation/Nav_t.hpp
        Filters/Fusion/State_t.hpp
        Filters/OutputFilter/OutputFilter.cpp
        Filters/OutputFilter/OutputFilter.hpp
        Utilities/Logger.hpp
        Devices/GPS/GpsSimulator.hpp
        Devices/GPS/GpsSimulator.cpp )

set(SOURCES_PI ${SOURCES}
        Devices/GPS/Gps.cpp
        Devices/GPS/Gps.hpp
        Devices/LoRa/LoRa.cpp
        Devices/LoRa/LoRa.hpp
        Devices/LoRa/RasPiBoards.h )

set(SOURCES_NO_PI ${SOURCES})

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_definitions(-DRASPBERRY_PI -DBCM2835_NO_DELAY_COMPATIBILITY)
    set(RADIOHEADBASE /home/pi/RadioHead)
    set(RADIOHEAD_SOURCES
            ${RADIOHEADBASE}/RHutil/RasPi.cpp
            ${RADIOHEADBASE}/RH_RF95.cpp
            ${RADIOHEADBASE}/RHDatagram.cpp
            ${RADIOHEADBASE}/RHHardwareSPI.cpp
            ${RADIOHEADBASE}/RHSPIDriver.cpp
            ${RADIOHEADBASE}/RHGenericDriver.cpp
            ${RADIOHEADBASE}/RHGenericSPI.cpp
            ${RADIOHEADBASE}/RHutil/RasPi.h
            ${RADIOHEADBASE}/RH_RF95.h
            ${RADIOHEADBASE}/RHDatagram.h
            ${RADIOHEADBASE}/RHHardwareSPI.h
            ${RADIOHEADBASE}/RHSPIDriver.h
            ${RADIOHEADBASE}/RHGenericDriver.h
            ${RADIOHEADBASE}/RHGenericSPI.h)
    include_directories(${RADIOHEADBASE})
    add_executable(FlightComputer ${SOURCES_PI} ${RADIOHEAD_SOURCES})
    target_link_libraries(FlightComputer pthread bcm2835)
ELSE(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_NO_PI})
    target_link_libraries(FlightComputer pthread)
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

