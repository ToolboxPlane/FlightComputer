cmake_minimum_required(VERSION 3.10)
project(FlightComputer)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif ()

set(SOURCES
        main.cpp
        Devices/Stream/Serial/Serial.cpp
        Devices/Stream/Tcp/TcpServer.cpp
        Devices/Stream/StreamDevice.cpp
        Devices/rcLib/RadioControlProtocol/rcLib.cpp
        Devices/rcLib/RcLibSimulator.cpp
        Filters/Fusion/Fusion.cpp
        Filters/Navigation/Navigation.cpp
        Filters/MeshManager/MeshManager.cpp
        Filters/OutputFilter/OutputFilter.cpp
        Devices/GPS/GpsSimulator.cpp
        Devices/GPS/Gps.cpp)

set(SOURCES_PI ${SOURCES}
        Devices/LoRa/LoRa.cpp)

set(SOURCES_NO_PI ${SOURCES})

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_definitions(-DRASPBERRY_PI)
    add_executable(FlightComputer ${SOURCES_PI})
    target_link_libraries(FlightComputer gps pthread wiringPi)
ELSE(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_NO_PI})
    target_link_libraries(${PROJECT_NAME} pthread gps)
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

