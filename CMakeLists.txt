cmake_minimum_required(VERSION 3.5)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
        main.cpp
        ChannelMultiplexer.hpp
        Channel.hpp
        ThreadModule.hpp
        Devices/FlightController/FlightController.cpp
        Devices/RadioControlProtocol/rcLib.cpp
        Devices/RcLibSimulator.cpp
        Devices/RcLibSimulator.hpp
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
        Utilities/Logger.hpp )

set(SOURCES_PI ${SOURCES}
        Devices/GPS/Gps.cpp
        Devices/GPS/Gps.hpp
        Devices/LoRa/LoRa.cpp
        Devices/LoRa/LoRa.hpp
        Devices/LoRa/RasPiBoards.h )

set(SOURCES_NO_PI ${SOURCES}
        Devices/GPS/GpsSimulator.hpp
        Devices/GPS/GpsSimulator.cpp )

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_PI})
    add_definitions(-D_ON_PI_)
ELSE(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_NO_PI})
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

target_link_libraries(FlightComputer pthread)