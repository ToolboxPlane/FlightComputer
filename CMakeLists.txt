cmake_minimum_required(VERSION 3.10)
project(FlightComputer)

find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif (NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Werror -mtune=native -march=native")
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        set(ASAN_FLAG "-static-libsan")
    else ()
        set(ASAN_FLAG "-static-libasan")
    endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0 ${ASAN_FLAG} -DDEBUG")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
endif ()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-error=unused-but-set-parameter")

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS GroundStation.proto)

set(SOURCES
        main.cpp
        Devices/rcLib/RadioControlProtocolCpp/rcLib.cpp
        Devices/rcLib/RadioControlProtocolCpp/RadioControlProtocolC/rc_lib.c
        Devices/rcLib/PackageUtil.cpp
        Devices/rcLib/RcLibSimulator.cpp
        Devices/GPS/Sim/GpsSimulator.cpp
        Devices/GPS/Node/Gps.cpp
        Devices/GPS/Type/GpsMeasurement_t.cpp
        Devices/Network/Network.cpp
        Devices/Serial/SerialPosix.cpp
        Devices/SRF02/Node/SRF02.cpp
        Devices/SRF02/Sim/SRF02Sim.cpp
        Filters/Fusion/Node/Fusion.cpp
        Filters/Fusion/Lib/StateEstimateParticleFilter.cpp
        Filters/Fusion/Lib/system.c
        Filters/Fusion/Lib/random_util.c
        Filters/Fusion/Lib/Calibration.cpp
        Filters/Navigation/Node/Navigation.cpp
        Filters/Navigation/Type/Waypoint_t.cpp
        Filters/MeshManager/MeshManager.cpp
        Filters/OutputFilter/OutputFilter.cpp
        Filters/FeedbackControl/Node/FeedbackControl.cpp
        Utilities/Recording/NameProvider.cpp
        Utilities/time.cpp
        ${PROTO_SRCS} ${PROTO_HDRS})

set(SOURCES_PI ${SOURCES}
        Devices/LoRa/LoRa.cpp)

set(SOURCES_NO_PI ${SOURCES})

set(LIBS gps pthread stdc++fs SI ${Protobuf_LIBRARIES})

if (${CMAKE_SYSTEM_PROCESSOR} MATCHES "arm")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++") # libstdc++6 is kind of fucked in Raspbian with GCC 10
    add_definitions(-DRASPBERRY_PI)
    add_executable(${PROJECT_NAME} ${SOURCES_PI})
    target_link_libraries(${PROJECT_NAME} ${LIBS} wiringPi)
else ()
    add_executable(${PROJECT_NAME} ${SOURCES_NO_PI})
    target_link_libraries(${PROJECT_NAME} ${LIBS})
endif ()

add_definitions(-DSI_DEFAULT_TYPE=float)
target_include_directories(${PROJECT_NAME} PUBLIC Utilities/SI)
add_subdirectory(Utilities/SI)
