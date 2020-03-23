#!/bin/bash

# Install necessary software
sudo apt install -y git gpsd libgps-dev wiringpi gpsd-clients

echo "Next enable SPI and UART via raspi-config"
