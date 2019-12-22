cmake_minimum_required(VERSION 3.10)
project(FlightComputer)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 -fsanitize=address")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-but-set-parameter")

set(SOURCES
        main.cpp
        Devices/rcLib/RadioControlProtocolCpp/rcLib.cpp
        Devices/rcLib/RadioControlProtocolCpp/RadioControlProtocolC/rc_lib.c
        Devices/rcLib/PackageOstream.cpp
        Devices/rcLib/RcLibSimulator.cpp
        Devices/GPS/Sim/GpsSimulator.cpp
        Devices/GPS/Node/Gps.cpp
        Devices/Network/Network.cpp
        Devices/Serial/SerialPosix.cpp
        Filters/Fusion/Node/Fusion.cpp
        Filters/Fusion/Lib/StateEstimate.cpp
        Filters/Fusion/Lib/system.c
        Filters/Fusion/Lib/random_util.c
        Filters/Navigation/Node/Navigation.cpp
        Filters/MeshManager/MeshManager.cpp
        Filters/OutputFilter/OutputFilter.cpp
        Filters/FeedbackControl/Node/FeedbackControl.cpp)

set(SOURCES_PI ${SOURCES}
        Devices/LoRa/LoRa.cpp)

set(SOURCES_NO_PI ${SOURCES})

set(LIBS gps pthread stdc++fs)

IF(${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    add_definitions(-DRASPBERRY_PI)
    add_executable(FlightComputer ${SOURCES_PI})
    target_link_libraries(FlightComputer ${LIBS} wiringPi)
ELSE()
    add_executable(FlightComputer ${SOURCES_NO_PI})
    target_link_libraries(${PROJECT_NAME} ${LIBS})
ENDIF()

