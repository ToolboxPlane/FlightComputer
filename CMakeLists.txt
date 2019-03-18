cmake_minimum_required(VERSION 3.10)
project(FlightComputer)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
endif ()

set(SOURCES
        main.cpp
        Devices/rcLib/RadioControlProtocol/rcLib.cpp
        Devices/rcLib/RcLibSimulator.cpp
        Devices/GPS/GpsSimulator.cpp
        Devices/GPS/Gps.cpp
        Devices/Network/Network.cpp
        Devices/Serial/SerialPosix.cpp
        Filters/Fusion/Fusion.cpp
        Filters/Navigation/Navigation.cpp
        Filters/MeshManager/MeshManager.cpp
        Filters/OutputFilter/OutputFilter.cpp
        Filters/FeedbackControl/FeedbackControl.cpp)

set(SOURCES_PI ${SOURCES}
        Devices/LoRa/LoRa.cpp)

set(SOURCES_NO_PI ${SOURCES})

set(LIBS gps pthread stdc++fs)

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_definitions(-DRASPBERRY_PI)
    add_executable(FlightComputer ${SOURCES_PI})
    target_link_libraries(FlightComputer ${LIBS} wiringPi)
ELSE(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_executable(FlightComputer ${SOURCES_NO_PI})
    target_link_libraries(${PROJECT_NAME} ${LIBS})
ENDIF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")

