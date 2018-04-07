#!/bin/bash

# Install necessary software
sudo apt install -y build-essential git cmake

# Install Radiohead
git clone https://github.com/hallard/RadioHead /home/pi/RadioHead

# Install bcm2835 Library (Library for the CPU of the Pi)
# See also: http://www.airspayce.com/mikem/bcm2835/
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.55.tar.gz
tar zxvf bcm2835-1.55.tar.gz
cd bcm2835-1.55
./configure
make -j4
sudo make check
sudo make install

# Generate a ssh-key
ssh-keygen
cat .ssh/id-rsa.pub

# Clone this repo
git clone git@github.com:ToolboxPlane/FlightComputer.git
cd FlightComputer
git submodule init
git submodule update

# Compile the program
mkdir build
cd build
cmake ..
make -j4
