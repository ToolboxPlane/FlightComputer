#!/bin/bash

# Install necessary software
sudo apt install -y build-essential git cmake

# Install Radiohead
git clone https://github.com/hallard/RadioHead /home/pi/RadioHead

# Install bcm2835 Library (Library for the CPU of the Pi)
# See also: http://www.airspayce.com/mikem/bcm2835/
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
tar zxvf bcm2835-1.52.tar.gz
cd bcm2835-1.52
./configure
make -j4
sudo make check
sudo make install

# Clone this repo
git clone git@github.com:ToolboxPlane/FlightComputer.git

# Compile the program
cd FlightComputer
mkdir build
cd build
cmake ..
make -j4
