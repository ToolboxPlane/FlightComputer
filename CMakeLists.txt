cmake_minimum_required(VERSION 3.9)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 17)

add_executable(FlightComputer main.cpp Devices/Serial/Serial.cpp Devices/RadioControlProtocol/rcLib.cpp ChannelMultiplexer.hpp Channel.hpp Devices/RcLibDebug.cpp Devices/RcLibDebug.hpp Devices/SerialSimulator.cpp Devices/SerialSimulator.hpp Fusion/Fusion.cpp Fusion/Fusion.hpp Fusion/FusionDebug.cpp Fusion/FusionDebug.hpp ThreadModule.hpp Navigation/Navigation.cpp Navigation/Navigation.hpp MeshManager/MeshManager.cpp MeshManager/MeshManager.hpp Devices/GPS/Gps.cpp Devices/GPS/Gps.hpp Devices/LoRa/LoRa.cpp Devices/LoRa/LoRa.hpp)
target_link_libraries(FlightComputer pthread)