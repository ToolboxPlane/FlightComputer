//
// Created by paul on 15.03.18.
//

#ifndef FLIGHTCOMPUTER_LORA_HPP
#define FLIGHTCOMPUTER_LORA_HPP

#include "LoRaHal.cpp"
#include "../../ThreadModule.hpp"
#include "../../Channel.hpp"

class LoRa : public ThreadModule{
public:
    LoRa() : ThreadModule(), loRaHal() {}

    Channel<rcLib::Package> &getChannelOut();
    Channel<rcLib::Package> &getChannelIn();
private:
    Channel<rcLib::Package> in, out;
    void run() override;
    LoRaHal loRaHal;
};


#endif //FLIGHTCOMPUTER_LORA_HPP
