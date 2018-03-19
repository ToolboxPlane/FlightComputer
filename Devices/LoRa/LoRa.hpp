//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_LORA_HPP
#define FLIGHTCOMPUTER_LORA_HPP

#include "LoRaHal.cpp"
#include "../../ThreadModule.hpp"
#include "../../Channel.hpp"
#include "RH_RF95.cpp"

class LoRa : public ThreadModule{
public:
    LoRa();

    Channel<rcLib::PackageExtended> &getChannelOut();
    Channel<rcLib::PackageExtended> &getChannelIn();
private:
    Channel<rcLib::PackageExtended> in, out;
    void run() override;
    uint8_t* buf;
    uint8_t len;
    RH_RF95 rf95;
};


#endif //FLIGHTCOMPUTER_LORA_HPP
