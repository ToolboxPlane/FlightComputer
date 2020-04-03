#!/bin/bash
EXECUTABLE=./cmake-build-toolbox-plane/FlightComputer
sudo setcap cap_net_raw=ep ${EXECUTABLE}
sudo chrt --rr 99 ${EXECUTABLE}
