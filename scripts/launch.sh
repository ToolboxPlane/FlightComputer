#!/bin/bash
sudo killall gpsd
gpsd /dev/ttyS0
sudo chrt --rr 99 ./build/FlightComputer

