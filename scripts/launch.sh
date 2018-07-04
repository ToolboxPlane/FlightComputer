#!/bin/bash
sudo killall gpsd
gpsd /dev/ttyS0 -S 3000
sudo chrt --rr 99 ./build/FlightComputer

