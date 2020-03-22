#!/bin/bash
#sudo killall gpsd
#gpsd /dev/ttyS0
setserial /dev/ttyUSB0 low_latency
sudo chrt --rr 99 ./cmake-build-release/FlightComputer

