#ifndef LORAHAL_H
#define LORAHAL_H

#include <bcm2835.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

#include <RH_RF69.h>
#include <RH_RF95.h>


class LoRaHal
{
public:
    LoRaHal();
    void waitForPackage();
    uint8_t* getData();
    size_t getLength();

private:
    uint8_t* buf;
    uint8_t len;
    RH_RF95 rf95;
};

#endif // LORAHAL_H
