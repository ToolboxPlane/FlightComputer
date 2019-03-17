#!/bin/bash

# Install necessary software
sudo apt install -y git docker

# Generate a ssh-key
ssh-keygen
cat .ssh/id-rsa.pub

# Clone this repo
git clone git@github.com:ToolboxPlane/FlightComputer.git
cd FlightComputer
git submodule init
git submodule update

# Setup the autostart
sudo cp autostart.sh /etc/init.d/FlightComputer
sudo chmod 755 /etc/init.d/FlightComputer

# Install ST-Link
cd ~
sudo apt install libusb-1.0-0 libusb-1.0-0-dev
git clone https://github.com/texane/stlink.git
cd stlink
mkdir build && cd build
cmake ..
make -j4
