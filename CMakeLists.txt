cmake_minimum_required(VERSION 3.5)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 14)

set(SOURCES
        main.cpp
        Channel.hpp
        Filter.hpp
        Devices/Stream/Serial/Serial.cpp
        Devices/Stream/Serial/Serial.hpp
        Devices/Stream/Tcp/TcpServer.cpp
        Devices/Stream/Tcp/TcpServer.hpp
        Devices/Stream/StreamDevice.cpp
        Devices/Stream/StreamDevice.hpp
        Devices/rcLib/RadioControlProtocol/rcLib.cpp
        Devices/rcLib/RadioControlProtocol/rcLib.hpp
        Devices/rcLib/PackageExtended.hpp
        Devices/rcLib/RcLibSimulator.cpp
        Devices/rcLib/RcLibSimulator.hpp
        Devices/GPS/GpsMeasurement_t.hpp
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
        Devices/GPS/GpsSimulator.cpp
        Devices/GPS/Gps.cpp
        Devices/GPS/Gps.hpp
        Utilities/ChannelRecorder.hpp
        Utilities/ChannelReplay.hpp
        Filters/Navigation/Waypoint_t.hpp
        Devices/GPS/Gps_t.hpp
        MultipleOutputChannel.hpp)

set(SOURCES_PI ${SOURCES}
        Devices/LoRa/LoRa.cpp
        Devices/LoRa/LoRa.hpp)

set(SOURCES_NO_PI ${SOURCES})

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_PI})
    target_link_libraries(FlightComputer gps pthread wiringPi)
ELSE(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_PI})
    target_link_libraries(FlightComputer pthread gps)
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

