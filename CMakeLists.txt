cmake_minimum_required(VERSION 3.9)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    main.cpp
    Devices/FlightController/FlightController.cpp
    Devices/RadioControlProtocol/rcLib.cpp
    ChannelMultiplexer.hpp
    Channel.hpp
        Devices/RcLibSimulator.cpp
        Devices/RcLibSimulator.hpp
    Fusion/Fusion.cpp
    Fusion/Fusion.hpp
    ThreadModule.hpp
    Navigation/Navigation.cpp
    Navigation/Navigation.hpp
    MeshManager/MeshManager.cpp
    MeshManager/MeshManager.hpp
        Devices/GPS/Gps_t.hpp
    Devices/Serial.cpp
    Devices/Serial.hpp
    Logger.hpp Navigation/Nav_t.hpp Fusion/State_t.hpp)

set(SOURCES_PI ${SOURCES},
    Devices/GPS/Gps.cpp
    Devices/GPS/Gps.hpp
    Devices/LoRa/LoRa.cpp
    Devices/LoRa/LoRa.hpp
    Devices/LoRa/LoRaHal.cpp
    Devices/LoRa/LoRaHal.hpp
    Devices/LoRa/RasPiBoards.h
)

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