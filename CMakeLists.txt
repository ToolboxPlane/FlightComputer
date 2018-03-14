cmake_minimum_required(VERSION 3.9)
project(FlightComputer)

set(CMAKE_CXX_STANDARD 17)

add_executable(FlightComputer main.cpp Serial/Serial.cpp Serial/RadioControlProtocol/rcLib.cpp ChannelMultiplexer.hpp Channel.hpp Serial/RcLibLogger.cpp Serial/RcLibLogger.hpp Serial/SerialSimulator.cpp Serial/SerialSimulator.hpp Fusion/Fusion.cpp Fusion/Fusion.hpp)
target_link_libraries(FlightComputer pthread)