#!/bin/bash

# Install necessary software
sudo apt update
sudo apt upgrade -y
sudo apt install -y gpsd libgps-dev gpsd-clients setserial tmux vim

# Enter the serial port for gpsd
sed 's/DEVICE=""/DEVICE="\/dev\/ttyS0"/g' /etc/default/gpsd | sudo tee /etc/default/gpsd > /dev/null

# Create Symlink for FC, PDB and US
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6015\", ATTRS{serial}==\"DO3L0842\", SYMLINK+=\"ttyFC\",RUN+=\"/bin/setserial /dev/ttyFC low_latency\"" | sudo tee /etc/udev/rules.d/20_FC.rules > /dev/null
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6015\", ATTRS{serial}==\"DO01S2MQ\", SYMLINK+=\"ttyPDB\",RUN+=\"/bin/setserial /dev/ttyPDB low_latency\"" | sudo tee /etc/udev/rules.d/20_PDB.rules > /dev/null
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"239a\", ATTRS{idProduct}==\"800b\", ATTRS{serial}==\"812ACF59504B5948382E314BFF10272E\", SYMLINK+=\"ttyNav\",RUN+=\"/bin/setserial /dev/ttyNav low_latency\"" | sudo tee /etc/udev/rules.d/20_Nav.rules > /dev/null
echo "SUBSYSTEM==\"tty\", ATTRS{idVendor}==\"0403\", ATTRS{idProduct}==\"6001\", ATTRS{serial}==\"A7007K9k\", SYMLINK+=\"ttyUS\"" | sudo tee /etc/udev/rules.d/20_US.rules > /dev/null

sudo /etc/init.d/udev restart

MACHINE_TYPE=$(uname -m)
if [ "${MACHINE_TYPE}" == 'armv7l' ]; then
  sudo apt install -y wiringpi
  echo "Next enable SPI and UART via raspi-config, then restart the system"
else
  echo "Installation for non-pi targets complete"
fi
