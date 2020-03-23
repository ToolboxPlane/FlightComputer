#!/bin/bash

# Install necessary software
sudo apt update
sudo apt upgrade -y
sudo apt install -y gpsd libgps-dev wiringpi gpsd-clients setserial

# Enter the serial port for gpsd
sed 's/DEVICE=""/DEVICE="\/dev\/ttyS0"/g' /etc/default/gpsd | sudo tee /etc/default/gpsd > /dev/null

# Create Symlink for FC, PDB and US
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6015\", ATTRS{serial}==\"DO3L0842\", SYMLINK+=\"ttyFC\"" | sudo tee /etc/udev/rules.d/20_FC.rules > /dev/null
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6015\", ATTRS{serial}==\"DO01S2MQ\", SYMLINK+=\"ttyPDB\"" | sudo tee /etc/udev/rules.d/20_PDB.rules > /dev/null
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6001\", ATTRS{serial}==\"A7007K9k\", SYMLINK+=\"ttyUS\"" | sudo tee /etc/udev/rules.d/20_US.rules > /dev/null

sudo /etc/init.d/udev restart

echo "Next enable SPI and UART via raspi-config, then restart the system"
