#!/bin/bash
# bcm2835 Library installieren
# http://www.airspayce.com/mikem/bcm2835/
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.52.tar.gz
tar zxvf bcm2835-1.52.tar.gz
cd bcm2835-1.52
./configure
make -j4
sudo make check
sudo make install

# Toolchain installieren

# Program clonen

# Program kompilieren
