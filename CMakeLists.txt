cmake_minimum_required(VERSION 3.9)
project(FlightComputerMonolithic)

set(CMAKE_CXX_STANDARD 17)

add_executable(FlightComputerMonolithic main.cpp Channel.hpp Serial.cpp Serial.hpp Filter.cpp Filter.hpp)